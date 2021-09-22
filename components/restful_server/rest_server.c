/* HTTP Restful API Server

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "rest_server.h"

#include <string.h>
#include <fcntl.h>
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "cJSON.h"



#include "led.h"
#include "nvsblob.h"
#include "ds18b20.h"

static const char *REST_TAG = "esp-rest";
#define REST_CHECK(a, str, goto_tag, ...)                                              \
    do                                                                                 \
    {                                                                                  \
        if (!(a))                                                                      \
        {                                                                              \
            ESP_LOGE(REST_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag;                                                             \
        }                                                                              \
    } while (0)

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)
#define SCRATCH_BUFSIZE (10240)

typedef struct rest_server_context {
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath)
{
    const char *type = "text/plain";
    if (CHECK_FILE_EXTENSION(filepath, ".html")) {
        type = "text/html";
    } else if (CHECK_FILE_EXTENSION(filepath, ".js")) {
        type = "application/javascript";
    } else if (CHECK_FILE_EXTENSION(filepath, ".css")) {
        type = "text/css";
    } else if (CHECK_FILE_EXTENSION(filepath, ".png")) {
        type = "image/png";
    } else if (CHECK_FILE_EXTENSION(filepath, ".ico")) {
        type = "image/x-icon";
    } else if (CHECK_FILE_EXTENSION(filepath, ".svg")) {
        type = "text/xml";
    }
    return httpd_resp_set_type(req, type);
}

/* Send HTTP response with the contents of the requested file */
static esp_err_t rest_common_get_handler(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];

    rest_server_context_t *rest_context = (rest_server_context_t *)req->user_ctx;
    strlcpy(filepath, rest_context->base_path, sizeof(filepath));
    if (req->uri[strlen(req->uri) - 1] == '/') {
        strlcat(filepath, "/index.html", sizeof(filepath));
        printf(filepath);
    } else {
        strlcat(filepath, req->uri, sizeof(filepath)); //when files stored on esp
        //strlcat(filepath, "/index.html", sizeof(filepath)); //when cdn - refresh button work
        printf(filepath);
    }
    int fd = open(filepath, O_RDONLY, 0);
    if (fd == -1) {
        ESP_LOGE(REST_TAG, "Failed to open file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }

    set_content_type_from_file(req, filepath);

    char *chunk = rest_context->scratch;
    ssize_t read_bytes;
    do {
        /* Read file in chunks into the scratch buffer */
        read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
        if (read_bytes == -1) {
            ESP_LOGE(REST_TAG, "Failed to read file : %s", filepath);
        } else if (read_bytes > 0) {
            /* Send the buffer contents as HTTP response chunk */
            if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK) {
                close(fd);
                ESP_LOGE(REST_TAG, "File sending failed!");
                /* Abort sending file */
                httpd_resp_sendstr_chunk(req, NULL);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (read_bytes > 0);
    /* Close file after sending complete */
    close(fd);
    ESP_LOGI(REST_TAG, "File sending complete");
    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* Simple handler for light brightness control */
static esp_err_t light_brightness_post_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE) {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len) {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0) {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);
    int red = cJSON_GetObjectItem(root, "red")->valueint;
    int green = cJSON_GetObjectItem(root, "green")->valueint;
    int blue = cJSON_GetObjectItem(root, "blue")->valueint;

    //gpio logic
    if (red < 100) {
        led_toggle();
    }
    else if (red > 100 && red < 150) {
        save_gpio_state(GPIO_LED, led_get_state(GPIO_LED));

    }

    ESP_LOGI(REST_TAG, "Light control: red = %d, green = %d, blue = %d", red, green, blue);
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}

/* handler for setting manual control (target temp OR target flow rate mode, set valve pos)*/
static esp_err_t manual_control_post_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE) {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len) {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0) {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);
    int targetTemp = cJSON_GetObjectItem(root, "targettemp")->valueint;
    char *heater = cJSON_GetObjectItem(root, "heater")->valuestring;
    char *pump = cJSON_GetObjectItem(root, "pump")->valuestring;

    if (strcmp(pump, "On") == 0) {
        printf("Pump: %s", pump);
        //pump function on
    } else {
        printf("Pump: %s", pump);
        printf("Target Temp: %d", targetTemp);
        //pump function off
    }


    //gpio logic
   // if (red < 100) {
     //   led_toggle();
  //  }
  //  else if (red > 100 && red < 150) {
   //     save_gpio_state(GPIO_LED, led_get_state(GPIO_LED));

 //   }

    //do something with target temp, update etc.

    ESP_LOGI(REST_TAG, "Target Temp: %d", targetTemp);
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}


    /* URI handler for saving setup 
    httpd_uri_t setup_save_post_uri = {
        .uri = "/api/v1/setup/save",
        .method = HTTP_POST,
        .handler = setup_save_post_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &setup_save_post_uri); */
/* handler for saving brewery setup*/
static esp_err_t setup_save_post_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE) {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len) {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0) {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);
    int kettlevolume = cJSON_GetObjectItem(root, "kettlevolume")->valueint;
    int mashtunvolume = cJSON_GetObjectItem(root, "mashtunvolume")->valueint;
    bool pumpedtransfer =cJSON_GetObjectItem(root, "pumpedtransfer")->valueint;
    char *units = cJSON_GetObjectItem(root, "units")->valuestring;
    int leadtime = cJSON_GetObjectItem(root, "leadtime")->valueint;
    char *heatingmethod = cJSON_GetObjectItem(root, "heatingmethod")->valuestring;
    char *coolingmethod = cJSON_GetObjectItem(root, "coolingmethod")->valuestring;

    int n = cJSON_GetArraySize(root);
    //printf("arraysize %d\n", n);

    // new version of ->valueint
    double x = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "mashtunvolume"));

    //->valuestring
    char *xx = cJSON_GetStringValue(cJSON_GetObjectItem(root, "mashtunvolume"));
    if (xx) {
          //  printf("mashtun item %s\n", xx)
        }

    printf("server print - kettlevolume: %d, mashtunvolume: %d, pumpedtransfer: %s, units: %s, leadtime: %d, heating: %s, cooling: %s\n",
            kettlevolume, mashtunvolume, pumpedtransfer ? "true" : "false", units, 
            leadtime, heatingmethod, coolingmethod);

           //load_brewery_setup();

            save_brewery_setup(kettlevolume, mashtunvolume, pumpedtransfer, units, 
            leadtime, heatingmethod, coolingmethod);

    ESP_LOGI(REST_TAG, "Kettle Volume: %d", kettlevolume);
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}

/* Simple handler for getting manual data (temp, valve position, flow rate*/
static esp_err_t temperature_data_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "raw", esp_random() % 20);
    printf("%lld adding random to chart\n", esp_timer_get_time() / 1000000);
    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}

/* Simple handler for getting system handler */
static esp_err_t setup_load_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();

   cJSON_AddNumberToObject(root, "kettlevolume", brewery_setup.kettle_volume);
   cJSON_AddNumberToObject(root, "mashtunvolume", brewery_setup.mash_tun_volume);
   cJSON_AddBoolToObject(root, "pumpedtransfer", brewery_setup.pumped_transfer);
   cJSON_AddStringToObject(root, "units", brewery_setup.units);
   cJSON_AddNumberToObject(root, "leadtime", brewery_setup.lead_time);
   cJSON_AddStringToObject(root, "heatingmethod", brewery_setup.heating_method);
   cJSON_AddStringToObject(root, "coolingmethod", brewery_setup.cooling_method);

    const char *brewery_setup = cJSON_Print(root);
    httpd_resp_sendstr(req, brewery_setup);
    free((void *)brewery_setup);
    cJSON_Delete(root);
    return ESP_OK;
}

/* Simple handler for getting system handler */
static esp_err_t system_info_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    cJSON_AddStringToObject(root, "version", IDF_VER);
    cJSON_AddNumberToObject(root, "cores", chip_info.cores);
    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}

/* Simple handler for getting temperature data */
static esp_err_t manual_data_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "raw", esp_random() % 20); //change to gettemp
    //cJSON_AddNumberToObject(root, "raw", ds18b20_get_temp()); //change to gettemp
    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}

esp_err_t start_rest_server(const char *base_path)
{
    REST_CHECK(base_path, "wrong base path", err);
    rest_server_context_t *rest_context = calloc(1, sizeof(rest_server_context_t));
    REST_CHECK(rest_context, "No memory for rest context", err);
    strlcpy(rest_context->base_path, base_path, sizeof(rest_context->base_path));

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(REST_TAG, "Starting HTTP Server");
    REST_CHECK(httpd_start(&server, &config) == ESP_OK, "Start server failed", err_start);

    /* URI handler for fetching system info */
    httpd_uri_t system_info_get_uri = {
        .uri = "/api/v1/system/info",
        .method = HTTP_GET,
        .handler = system_info_get_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &system_info_get_uri);

    /* URI handler for fetching temperature data */
    httpd_uri_t temperature_data_get_uri = {
        .uri = "/api/v1/temp/raw",
        .method = HTTP_GET,
        .handler = temperature_data_get_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &temperature_data_get_uri);

    /* URI handler for light brightness control */
    httpd_uri_t light_brightness_post_uri = {
        .uri = "/api/v1/light/brightness",
        .method = HTTP_POST,
        .handler = light_brightness_post_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &light_brightness_post_uri);

    /* URI handler for setting manual control */
    httpd_uri_t manual_control_post_uri = {
        .uri = "/api/v1/manual/brightness",
        .method = HTTP_POST,
        .handler = manual_control_post_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &manual_control_post_uri);

    /* URI handler for fetching manual control data */
    httpd_uri_t manual_data_get_uri = {
        .uri = "/api/v1/manual/raw",
        .method = HTTP_GET,
        .handler = manual_data_get_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &manual_data_get_uri);

    /* URI handler for saving setup */
    httpd_uri_t setup_save_post_uri = {
        .uri = "/api/v1/setup/save",
        .method = HTTP_POST,
        .handler = setup_save_post_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &setup_save_post_uri);

    
    /* URI handler for fetching brewery setup */
    httpd_uri_t setup_load_get_uri = {
        .uri = "/api/v1/setup/load",
        .method = HTTP_GET,
        .handler = setup_load_get_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &setup_load_get_uri);

    /* URI handler for getting web server files */
    httpd_uri_t common_get_uri = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = rest_common_get_handler,
        .user_ctx = rest_context
    };
    httpd_register_uri_handler(server, &common_get_uri);

    

    return ESP_OK;
err_start:
    free(rest_context);
err:
    return ESP_FAIL;
}
