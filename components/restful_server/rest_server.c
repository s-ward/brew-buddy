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

#include "EquipConfig.h"
#include "ActiveRecipe.h"
#include "BrewStates.h"

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

typedef struct rest_server_context
{
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath)
{
    const char *type = "text/plain";
    if (CHECK_FILE_EXTENSION(filepath, ".html"))
    {
        type = "text/html";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".js"))
    {
        type = "application/javascript";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".css"))
    {
        type = "text/css";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".png"))
    {
        type = "image/png";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".ico"))
    {
        type = "image/x-icon";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".svg"))
    {
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
    if (req->uri[strlen(req->uri) - 1] == '/')
    {
        strlcat(filepath, "/index.html", sizeof(filepath));
        printf(filepath);
    }
    else
    {
        strlcat(filepath, req->uri, sizeof(filepath)); //when files stored on esp
        //strlcat(filepath, "/index.html", sizeof(filepath)); //when cdn - refresh button work
        printf(filepath);
    }
    int fd = open(filepath, O_RDONLY, 0);
    if (fd == -1)
    {
        ESP_LOGE(REST_TAG, "Failed to open file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }

    set_content_type_from_file(req, filepath);

    char *chunk = rest_context->scratch;
    ssize_t read_bytes;
    do
    {
        /* Read file in chunks into the scratch buffer */
        read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
        if (read_bytes == -1)
        {
            ESP_LOGE(REST_TAG, "Failed to read file : %s", filepath);
        }
        else if (read_bytes > 0)
        {
            /* Send the buffer contents as HTTP response chunk */
            if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK)
            {
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
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0)
        {
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
    if (red < 100)
    {
        led_toggle();
    }
    else if (red > 100 && red < 150)
    {
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
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0)
        {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);
    char *mode = cJSON_GetObjectItem(root, "mode")->valuestring;
    int heater_power = cJSON_GetObjectItem(root, "heaterpower")->valueint;
    int target_temp = cJSON_GetObjectItem(root, "targettemp")->valueint;
    int target_flow = cJSON_GetObjectItem(root, "targetflow")->valueint;

    int pump = cJSON_GetObjectItem(root, "pump")->valueint; // off = 0, ,on = 1
    int heater = cJSON_GetObjectItem(root, "heater")->valueint; 
    int valve1 = cJSON_GetObjectItem(root, "valve1")->valueint; // internal = 0, external = 1
    int valve2 = cJSON_GetObjectItem(root, "valve2")->valueint;
    int valve3 = cJSON_GetObjectItem(root, "valve3")->valueint;


    // send params to boonies manual function.
    //void Manual_Config(int valve1, int valve2, int valve3, int targetflow1, int targetflow2, int targetflow3, int pump, 
    //    int heater, int heater_power, int temp_sensor, int target_temp, int time, int volume);
    
    // mode switch - this way allows for expansion of available modes in future
    // mode None - Manual Mode Full_Man with target_temp = 0
    if (strcmp(mode, "None") == 0)
    {
        // do i need to set ManState?? e.g. ManState = Full_Man; do i change this or automatic
        ManState = Full_Man; 
        target_temp = 0;
        Manual_Config(valve1, valve2, valve3, target_flow, 100, 100, pump, heater, heater_power, 2, target_temp, 0, 0);
    }
    // mode Temperature Control - Manual Mode Full_Man with target_temp > 0
    else if (strcmp(mode, "Temperature Control") == 0)
    {
        ManState = Full_Man;
        Manual_Config(valve1, valve2, valve3, target_flow, 100, 100, pump, heater, heater_power, 2, target_temp, 0, 0);
    };
   
    // go to manual state
    // #include BrewStates.h
    Manual_In = 1; //OR?
    // BrewState = Manual_State;
  
    //print to terminal
    ESP_LOGI(REST_TAG, "Mode: %s, Heater Power: %d, Target Temp: %d, Target Flow: %d, Pump: %d, Heater: %d, Valve1: %d, Valve2: %d, Valve3: %d, Man State: %d, Brew State: %d",
        mode, heater_power, target_temp, target_flow, pump, heater, valve1, valve2, valve3, ManState, BrewState);
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}

/* handler for changing state*/
static esp_err_t state_change_post_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0)
        {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);
    // char *brewstate = cJSON_GetObjectItem(root, "brewstate")->valuestring;


    // if (strcmp(brewstate, "passive") == 0)
    // {
    //    BrewState = Passive_State;
    // }
    // int brewstate = cJSON_GetObjectItem(root, "brewstate")->valueint;
    int pauseint = cJSON_GetObjectItem(root, "pauseint")->valueint;
    int cancelint = cJSON_GetObjectItem(root, "cancelint")->valueint;
    int brewint = cJSON_GetObjectItem(root, "brewint")->valueint;
    int cleanint = cJSON_GetObjectItem(root, "cleanint")->valueint;
    int userintreq = cJSON_GetObjectItem(root, "userintreq")->valueint;
    int adjunctreq = cJSON_GetObjectItem(root, "adjunctreq")->valueint;
    //int manualint = cJSON_GetObjectItem(root, "manualint")->valueint;

   // if (brewstate == 3){
   //     BrewState = Passive_State;
  //  } else if (brewstate == 4) {
   //     BrewState = Safety_Check_State;
   // }
    // really is just
    // BrewState = brewstate;
    Pause_In = pauseint;
    Cancel_In = cancelint;
    Brew_In = brewint;
    Clean_In = cleanint;
    User_Int_Rqd = userintreq;
    User_Adjunct_Rqd = adjunctreq;
   

    ESP_LOGI(REST_TAG, "State POST - Brew State: %d, Pause In: %d, Cancel In: %d", BrewState, Pause_In, Cancel_In);
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}

/* handler for saving brewery setup*/
static esp_err_t setup_save_post_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0)
        {
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
    bool pumpedtransfer = cJSON_GetObjectItem(root, "pumpedtransfer")->valueint;
    char *units = cJSON_GetObjectItem(root, "units")->valuestring;
    int leadtime = cJSON_GetObjectItem(root, "leadtime")->valueint;
    char *heatingmethod = cJSON_GetObjectItem(root, "heatingmethod")->valuestring;
    char *coolingmethod = cJSON_GetObjectItem(root, "coolingmethod")->valuestring;

    save_brewery_setup(kettlevolume, mashtunvolume, pumpedtransfer, units,
                       leadtime, heatingmethod, coolingmethod);

    ESP_LOGI(REST_TAG, "Brewery Setup Saved");
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}

/* handler for saving recipe list */
static esp_err_t recipes_save_post_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0)
        {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);

    char *recipelist = cJSON_Print(root);

    save_recipe_list(recipelist);

    //ESP_LOGI(REST_TAG, "Recipes Saved: %s", recipelist);
    ESP_LOGI(REST_TAG, "Recipes Saved");
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}

/* start brew handler */
static esp_err_t start_brew_post_handler(httpd_req_t *req)
{

    printf("Start Brew Post Handler");

    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0)
        {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);

    int grainweight = cJSON_GetObjectItem(root, "grainweight")->valueint;
    int mashwatervol = cJSON_GetObjectItem(root, "mashwatervol")->valueint;
    int striketemp = cJSON_GetObjectItem(root, "striketemp")->valueint;
    
    cJSON *mashstages = cJSON_GetObjectItem(root, "mashstages"); //array of mashstages

    cJSON *mashstage1 = cJSON_GetArrayItem(mashstages, 0); //mash stage 1
    int mashtime1 = cJSON_GetObjectItem(mashstage1, "time")->valueint;
    int mashtemp1 = cJSON_GetObjectItem(mashstage1, "temp")->valueint;

    cJSON *mashstage2 = cJSON_GetArrayItem(mashstages, 1); //mash stage 2
    int mashtime2 = cJSON_GetObjectItem(mashstage2, "time")->valueint;
    int mashtemp2 = cJSON_GetObjectItem(mashstage2, "temp")->valueint;

    cJSON *mashstage3 = cJSON_GetArrayItem(mashstages, 2); //mash stage 3
    int mashtime3 = cJSON_GetObjectItem(mashstage3, "time")->valueint;
    int mashtemp3 = cJSON_GetObjectItem(mashstage3, "temp")->valueint;

    cJSON *mashstage4 = cJSON_GetArrayItem(mashstages, 3); //mash stage 4
    int mashtime4 = cJSON_GetObjectItem(mashstage4, "time")->valueint;
    int mashtemp4 = cJSON_GetObjectItem(mashstage4, "temp")->valueint;

    cJSON *mashstage5 = cJSON_GetArrayItem(mashstages, 4); //mash stage 5
    int mashtime5 = cJSON_GetObjectItem(mashstage5, "time")->valueint;
    int mashtemp5 = cJSON_GetObjectItem(mashstage5, "temp")->valueint;
    
   
    int flowrate = cJSON_GetObjectItem(root, "flowrate")->valueint;
    int spargewatervol = cJSON_GetObjectItem(root, "spargewatervol")->valueint;
    int spargetemp = cJSON_GetObjectItem(root, "spargetemp")->valueint;
    int boiltime = cJSON_GetObjectItem(root, "boiltime")->valueint;
    
    cJSON *adjuncts = cJSON_GetObjectItem(root, "adjuncts"); //array of adjuncts

    cJSON *adjunct1 = cJSON_GetArrayItem(adjuncts, 0); //adjunct 1
    int adjuncttime1 = cJSON_GetObjectItem(adjunct1, "time")->valueint;
    // char* adjunctname1 = cJSON_GetObjectItem(adjunct1, "adjuncts")->valuestring;
    char* adjunctname1;

    // new version of ->valuestring
    // char *adjunctname1 = cJSON_GetStringValue(cJSON_GetObjectItem(adjunct1, "adjuncts"));
    // if (adjunctname1) {
    // }
    if (cJSON_HasObjectItem(adjunct1, "adjuncts")) {
        adjunctname1 = cJSON_GetObjectItem(adjunct1, "adjuncts")->valuestring;
    } else {
        adjunctname1 = "";
    }
   

    cJSON *adjunct2 = cJSON_GetArrayItem(adjuncts, 1); //adjunct 2
    int adjuncttime2 = cJSON_GetObjectItem(adjunct2, "time")->valueint;
    // char* adjunctname2 = cJSON_GetObjectItem(adjunct2, "adjuncts")->valuestring;
    char* adjunctname2;

    if (cJSON_HasObjectItem(adjunct2, "adjuncts")) {
        adjunctname2 = cJSON_GetObjectItem(adjunct2, "adjuncts")->valuestring;
    } else {
        adjunctname2 = "";
    }

    cJSON *adjunct3 = cJSON_GetArrayItem(adjuncts, 2); //adjunct 3
    int adjuncttime3 = cJSON_GetObjectItem(adjunct3, "time")->valueint;
    // char* adjunctname3 = cJSON_GetObjectItem(adjunct3, "adjuncts")->valuestring;
    char* adjunctname3;
    if (cJSON_HasObjectItem(adjunct3, "adjuncts")) {
        adjunctname3 = cJSON_GetObjectItem(adjunct3, "adjuncts")->valuestring;
    } else {
        adjunctname3 = "";
    }

    cJSON *adjunct4 = cJSON_GetArrayItem(adjuncts, 3); //adjunct 4
    int adjuncttime4 = cJSON_GetObjectItem(adjunct4, "time")->valueint;
    // char* adjunctname4 = cJSON_GetObjectItem(adjunct4, "adjuncts")->valuestring;
    char* adjunctname4;
    if (cJSON_HasObjectItem(adjunct4, "adjuncts")) {
        adjunctname4 = cJSON_GetObjectItem(adjunct4, "adjuncts")->valuestring;
    } else {
        adjunctname4 = "";
    }

    cJSON *adjunct5 = cJSON_GetArrayItem(adjuncts, 4); //adjunct 5
    int adjuncttime5 = cJSON_GetObjectItem(adjunct5, "time")->valueint;
    // char* adjunctname5 = cJSON_GetObjectItem(adjunct5, "adjuncts")->valuestring;
    char* adjunctname5;
    if (cJSON_HasObjectItem(adjunct5, "adjuncts")) {
        adjunctname5 = cJSON_GetObjectItem(adjunct5, "adjuncts")->valuestring;
    } else {
        adjunctname5 = "";
    }
    
    int cooltemp = cJSON_GetObjectItem(root, "cooltemp")->valueint;

    // #include ActiveRecipe
    ActiveRecipe (grainweight, mashwatervol, striketemp, flowrate,
        mashtime1, mashtime2, mashtime3, mashtime4, mashtime5,
        mashtemp1, mashtemp2, mashtemp3, mashtemp4, mashtemp5,
        spargewatervol, spargetemp, boiltime,
        adjunctname1, adjunctname2, adjunctname3, adjunctname4, adjunctname5,
        adjuncttime1, adjuncttime2, adjuncttime3, adjuncttime4, adjuncttime5,
        cooltemp);

    // #include BrewStates.h
    // call to change to brew state
    // BrewState = Safety_Check_State or
    // Brew_In = 1;

  //  ESP_LOGI(REST_TAG, "Recipes Saved: %s", recipelist);
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}

/* Simple handler for getting manual data (temp, valve position, flow rate) */
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

/* Simple handler for getting brewery setup */
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

/* Simple handler for getting recipe list */
static esp_err_t recipe_list_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");

    printf("recipes in rest_server.c at top:\n");

    // create JSON object example
    // cJSON *root = cJSON_CreateObject();
    // cJSON *recipes = cJSON_CreateArray();
    // cJSON *recipe;

    // cJSON_AddItemToObject(root, "recipes", recipes);

    // cJSON_AddItemToArray(recipes, recipe = cJSON_CreateObject());
    // cJSON_AddItemToObject(recipe, "name", cJSON_CreateString("Sam 1"));
    // cJSON_AddItemToObject(recipe, "weight", cJSON_CreateNumber(10)); // cJSON_CreateString("bmw123"));

    // cJSON_AddItemToArray(recipes, recipe = cJSON_CreateObject());
    // cJSON_AddItemToObject(recipe, "name", cJSON_CreateString("Sam 2"));
    // cJSON_AddItemToObject(recipe, "calories", cJSON_CreateNumber(100)); //cJSON_CreateString("mercedes123"));

    // char *out = cJSON_Print(root);
    // printf("root %s\n", out);
    //  httpd_resp_sendstr(req, out);
    // free(out);
    // cJSON_Delete(root);

    cJSON *root = cJSON_Parse(get_esp_recipe_list());
    const char *recipes = cJSON_Print(root);

    //printf("recipes in rest_server.c string: %s\n", recipes);

    httpd_resp_sendstr(req, recipes);
    free((void *)recipes);
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

    //printf("system info in rest_server.c string: %s\n", sys_info);

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
    //cJSON_AddNumberToObject(root, "raw", esp_random() % 20); //change to gettemp
    cJSON_AddNumberToObject(root, "rawTemp1", Current_Temp); //change to gettemp
    cJSON_AddNumberToObject(root, "rawFlow1", Flow_Rate); //change to gettemp

    //cJSON_AddNumberToObject(root, "raw", ds18b20_get_temp()); //change to gettemp
    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}

/* handler for getting brew progress data */
static esp_err_t brew_progress_get_handler(httpd_req_t *req)
{
    //printf("Brew progress get handler");

    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "brewstate", BrewState);
    cJSON_AddItemToObject(root, "autoprocess", cJSON_CreateString(Auto_Process));
    cJSON_AddItemToObject(root, "stage", cJSON_CreateString(Stage));
    cJSON_AddItemToObject(root, "step", cJSON_CreateString(Step));
    //cJSON_AddNumberToObject(root, "status", Status);
    cJSON_AddNumberToObject(root, "minutesremaining", Minutes_Remaining);
    // cJSON_AddNumberToObject(root, "secondsremaining", esp_random() % 60); // change to Seconds_Remaining
    cJSON_AddNumberToObject(root, "secondsremaining", Seconds_Remaining);
    cJSON_AddItemToObject(root, "targettemp", cJSON_CreateString(Target_Temperature));
    cJSON_AddItemToObject(root, "userintreqmessage", cJSON_CreateString(User_Int_Required));
    //cJSON_AddNumberToObject(root, "userintreq", User_Int_Required_Int);
    cJSON_AddNumberToObject(root, "userintreq", User_Int_Rqd);
    cJSON_AddItemToObject(root, "adjunctreqmessage", cJSON_CreateString(User_Adjunct_Required));
    //cJSON_AddNumberToObject(root, "adjunctreq", User_Adjunct_Required_Int);
    cJSON_AddNumberToObject(root, "adjunctreq", User_Adjunct_Rqd);

    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}

/* handler for getting state */
static esp_err_t state_get_handler(httpd_req_t *req)
{
    //printf("Brew progress get handler");

    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "brewstate", BrewState);
    cJSON_AddNumberToObject(root, "manualint", Manual_In);
    cJSON_AddNumberToObject(root, "pauseint", Pause_In);
    cJSON_AddNumberToObject(root, "cancelint", Cancel_In);
    cJSON_AddNumberToObject(root, "cleanint", Clean_In);
    cJSON_AddNumberToObject(root, "brewint", Brew_In);
  
    cJSON_AddNumberToObject(root, "userintreq", User_Int_Rqd);
    cJSON_AddNumberToObject(root, "adjunctreq", User_Adjunct_Rqd);

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
    config.max_uri_handlers = 20;
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(REST_TAG, "Starting HTTP Server");
    REST_CHECK(httpd_start(&server, &config) == ESP_OK, "Start server failed", err_start);

    /* URI handler for fetching system info */
    httpd_uri_t system_info_get_uri = {
        .uri = "/api/v1/system/info1",
        .method = HTTP_GET,
        .handler = system_info_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &system_info_get_uri);

    /* URI handler for fetching temperature data */
    httpd_uri_t temperature_data_get_uri = {
        .uri = "/api/v1/temp/raw",
        .method = HTTP_GET,
        .handler = temperature_data_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &temperature_data_get_uri);

    /* URI handler for light brightness control */
    httpd_uri_t light_brightness_post_uri = {
        .uri = "/api/v1/light/brightness",
        .method = HTTP_POST,
        .handler = light_brightness_post_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &light_brightness_post_uri);

    /* URI handler for setting manual control */
    httpd_uri_t manual_control_post_uri = {
        .uri = "/api/v1/manual/set",
        .method = HTTP_POST,
        .handler = manual_control_post_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &manual_control_post_uri);

    /* URI handler for setting state */
    httpd_uri_t state_change_post_uri = {
        .uri = "/api/v1/changestate",
        .method = HTTP_POST,
        .handler = state_change_post_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &state_change_post_uri);

    /* URI handler for fetching manual control data */
    httpd_uri_t manual_data_get_uri = {
        .uri = "/api/v1/manual/raw",
        .method = HTTP_GET,
        .handler = manual_data_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &manual_data_get_uri);

    /* URI handler for saving setup */
    httpd_uri_t setup_save_post_uri = {
        .uri = "/api/v1/setup/save",
        .method = HTTP_POST,
        .handler = setup_save_post_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &setup_save_post_uri);

        /* uri handler for startbrew */
    httpd_uri_t start_brew_post_uri = {
        .uri = "/api/v1/brew/start",
        .method = HTTP_POST,
        .handler = start_brew_post_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &start_brew_post_uri);

    /* URI handler for fetching brewery setup */
    httpd_uri_t setup_load_get_uri = {
        .uri = "/api/v1/setup/load",
        .method = HTTP_GET,
        .handler = setup_load_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &setup_load_get_uri);

    // /* URI handler for fetching recipe list string */
    httpd_uri_t recipe_list_get_uri = {
        .uri = "/api/v1/book/load",
        .method = HTTP_GET,
        .handler = recipe_list_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &recipe_list_get_uri);

    /* URI handler for saving recipe list to nvs */
    httpd_uri_t recipes_save_post_uri = {
        .uri = "/api/v1/book/save",
        .method = HTTP_POST,
        .handler = recipes_save_post_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &recipes_save_post_uri);

    /* URI handler for fetching brew progress data */
    httpd_uri_t brew_progress_get_uri = {
        .uri = "/api/v1/progress/data",
        .method = HTTP_GET,
        .handler = brew_progress_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &brew_progress_get_uri);

    /* URI handler for fetching brew states data */
    httpd_uri_t state_get_uri = {
        .uri = "/api/v1/getstate",
        .method = HTTP_GET,
        .handler = state_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &state_get_uri);

    /* URI handler for getting web server files */
    httpd_uri_t common_get_uri = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = rest_common_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &common_get_uri);

    return ESP_OK;
err_start:
    free(rest_context);
err:
    return ESP_FAIL;
}
