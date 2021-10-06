#include <stdio.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "driver/gpio.h"
#include <string.h>
#include "BrewStates.h"
#include "EquipConfig.h"

#include "nvsblob.h"

#define STORAGE_NAMESPACE "storage" 
#define BREWERY_SETUP_NAMESPACE "setup"

#define RECIPES_NAMESPACE "recipes"

//save gpio state, takes in gpio num (wps_led_get_gpio) and gpio state (wps_led_get_state);
//name space "gpio_states"
//key: gpio pin (get_gpio), value: state (get_state)
//this is for gpio bools off/on, perhaps needing to save analog values might need a different function save_gpio_value

/* Save the number of module restarts in NVS
   by first reading and then incrementing
   the number that has been saved previously.

   Return an error if anything goes wrong
   during this process.
 */
//esp_err_t save_restart_counter(void)
//esp_err_t save_gpio_state(int gpio_num, int/bool gpio_state)
//esp_err_t save_gpio_state(int gpio_num)
esp_err_t save_gpio_state(int gpio_num, int gpio_state)
//esp_err_t save_gpio_state(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
        return err;

    // Read
    //int32_t restart_counter = 1; // value will default to 0, if not set yet in NVS
    //int32_t restart_counter = gpio_state;
    //int32_t gpio_state = 0; // value will default to 0, if not set yet in NVS
    // err = nvs_get_i32(my_handle, "restart_conter", &restart_counter);
    // if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;

    // Write
    //restart_counter++;
    //err = nvs_set_i32(my_handle, "restart_conter", restart_counter);
    err = nvs_set_i32(my_handle, "restart_conter", gpio_state); ///change key to gpio num
    if (err != ESP_OK)
        return err;

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    err = nvs_commit(my_handle);
    if (err != ESP_OK)
        return err;

    // Close
    nvs_close(my_handle);
    printf("gpio_state saved: %d", gpio_state);
    return ESP_OK;
}

//read gpio state, used to set state at startup. takes gpio num wps_led_get_gpio and returns state
// do i set state in here allowing me to keep error codes? or change return to state and then set state in main

/* Read from NVS and print restart counter
   and the table with run times.
   Return an error if anything goes wrong
   during this process.
 */
//esp_err_t print_what_saved(void)
esp_err_t load_gpio_state(int gpio_num)
//esp_err_t load_gpio_state(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
        return err;

    // Read restart counter
    int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
    err = nvs_get_i32(my_handle, "restart_conter", &restart_counter);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;
    printf("Restart counter = %d\n", restart_counter);

    gpio_set_level(2, restart_counter);
    printf("Toggle LED\n");

    // Read run time blob
    size_t required_size = 0; // value will default to 0, if not set yet in NVS
    // obtain required memory space to store blob being read from NVS
    err = nvs_get_blob(my_handle, "run_time", NULL, &required_size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;
    printf("Run time:\n");
    if (required_size == 0)
    {
        printf("Nothing saved yet!\n");
    }
    else
    {
        uint32_t *run_time = malloc(required_size);
        err = nvs_get_blob(my_handle, "run_time", run_time, &required_size);
        if (err != ESP_OK)
        {
            free(run_time);
            return err;
        }
        for (int i = 0; i < required_size / sizeof(uint32_t); i++)
        {
            printf("%d: %d\n", i + 1, run_time[i]);
        }
        free(run_time);
    }

    // Close
    nvs_close(my_handle);
    return ESP_OK;
}

esp_err_t save_brewery_setup(int kettlevolume, int mashtunvolume, bool pumpedtransfer,
                             char *units, int leadtime, char *heatingmethod, char *coolingmethod)
//esp_err_t save_gpio_state(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    printf("save print - kettlevolume: %d, mashtunvolume: %d, pumpedtransfer: %s, units: %s, leadtime: %d, heating: %s, cooling: %s\n",
           kettlevolume, mashtunvolume, pumpedtransfer ? "true" : "false", units,
           leadtime, heatingmethod, coolingmethod);

    // Open
    err = nvs_open(BREWERY_SETUP_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
        return err;

    // Read
    //int32_t restart_counter = 1; // value will default to 0, if not set yet in NVS
    //int32_t restart_counter = gpio_state;
    //int32_t gpio_state = 0; // value will default to 0, if not set yet in NVS
    // err = nvs_get_i32(my_handle, "restart_conter", &restart_counter);
    // if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;

    // Write
    //restart_counter++;
    //err = nvs_set_i32(my_handle, "restart_conter", restart_counter);
    err = nvs_set_str(my_handle, "units", units); ///change key to gpio num
    if (err != ESP_OK)
        return err;
    err = nvs_set_str(my_handle, "heatmethod", heatingmethod); ///change key to gpio num
    if (err != ESP_OK)
        return err;
    err = nvs_set_str(my_handle, "coolmethod", coolingmethod); ///change key to gpio num
    if (err != ESP_OK)
        return err;
    err = nvs_set_i32(my_handle, "kettlevol", kettlevolume); ///change key to gpio num
    if (err != ESP_OK)
        return err;
    err = nvs_set_i32(my_handle, "mashtunvol", mashtunvolume); ///change key to gpio num
    if (err != ESP_OK)
        return err;
    err = nvs_set_i32(my_handle, "pumptrans", pumpedtransfer); ///change key to gpio num
    if (err != ESP_OK)
        return err;
    err = nvs_set_i32(my_handle, "leadtime", leadtime); ///change key to gpio num
    if (err != ESP_OK)
        return err;

    //test for recipe list
    // err = nvs_set_str(my_handle, "recipelist", "[{name: 'sam1', calories: 10}, {name: 'sam2', calories: 100}]"); ///change key to gpio num
    // if (err != ESP_OK)
    //     return err;

    update_brewery_setup(kettlevolume, mashtunvolume, pumpedtransfer, units, leadtime,
                         heatingmethod, coolingmethod);

    //update_recipe_list("[{name: 'sam1', calories: 10}, {name: 'sam2', calories: 100}]");
    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    err = nvs_commit(my_handle);
    if (err != ESP_OK)
        return err;

    // Close
    nvs_close(my_handle);
    printf("gpio_state saved: %d", 1);
    return ESP_OK;
}

/* Read from NVS and print restart counter
   and the table with run times.
   Return an error if anything goes wrong
   during this process.
 */
esp_err_t load_brewery_setup(void)
{

    printf("Start of load brewery setup\n");

    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(BREWERY_SETUP_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
        return err;

    printf("load brewery nvs opened succcess\n");

    // Read ints
    int32_t kettle_volume = 0; // default value if not stored in nvs
    int32_t mash_tun_volume = 0;
    int32_t pumped_transfer = 0;
    int32_t lead_time = 5;

    err = nvs_get_i32(my_handle, "kettlevol", &kettle_volume);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;

    printf("load brewery kettlevol load succcess\n");

    err = nvs_get_i32(my_handle, "mashtunvol", &mash_tun_volume);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;
    err = nvs_get_i32(my_handle, "pumptransr", &pumped_transfer);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;
    err = nvs_get_i32(my_handle, "leadtime", &lead_time);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;

    printf("load brewery leadtime load succcess\n");

    //read strings
    char *units;
    char *heating_method;
    char *cooling_method;

    size_t req_size = 0;

    err = nvs_get_str(my_handle, "units", NULL, &req_size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;
    if (req_size == 0)
    {
        // default value if not stored in nvs
        printf("Nothing saved yet! - brewery load\n");
        units = malloc(strlen("Metric") + 1);
        strcpy(units, "Metric");
    }
    else
    {
        units = malloc(req_size);
        err = nvs_get_str(my_handle, "units", units, &req_size);
        if (err != ESP_OK)
        {
            free(units);
            return err;
        }
    }

    printf("load brewery units load succcess\n");

    err = nvs_get_str(my_handle, "heatmethod", NULL, &req_size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;
    if (req_size == 0)
    {
        heating_method = malloc(strlen("RIMS") + 1);
        strcpy(heating_method, "RIMS");
    }
    else
    {
        heating_method = malloc(req_size);
        err = nvs_get_str(my_handle, "heatmethod", heating_method, &req_size);
        if (err != ESP_OK)
        {
            free(heating_method);
            return err;
        }
    }

    err = nvs_get_str(my_handle, "coolmethod", NULL, &req_size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        return err;
    if (req_size == 0)
    {
        cooling_method = malloc(strlen("None") + 1);
        strcpy(cooling_method, "None");
    }
    else
    {
        cooling_method = malloc(req_size);
        err = nvs_get_str(my_handle, "coolmethod", cooling_method, &req_size);
        if (err != ESP_OK)
        {

            free(cooling_method);
            return err;
        }
    }

    printf("load print after - kettlevolume: %d, mashtunvolume: %d, pumpedtransfer: %s, units: %s, leadtime: %d, heating: %s, cooling: %s\n",
           kettle_volume, mash_tun_volume, pumped_transfer ? "true" : "false", units,
           lead_time, heating_method, cooling_method);

    //do stuff with data before freeing mallocs
    //set boonies globals
    update_brewery_setup(kettle_volume, mash_tun_volume, pumped_transfer, units, lead_time,
                         heating_method, cooling_method);

    free(units);
    free(heating_method);
    free(cooling_method);

    // Close
    nvs_close(my_handle);
    return ESP_OK;
}

esp_err_t save_recipe_list(char *recipelist)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(BREWERY_SETUP_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
        return err;

    // Write
    err = nvs_set_str(my_handle, "recipelist", recipelist);
    if (err != ESP_OK)
        return err;

    // update_recipe_list(recipelist);

    err = nvs_commit(my_handle);
    if (err != ESP_OK)
        return err;

    update_recipe_list(recipelist);

    // Close
    nvs_close(my_handle);
    printf("nvsblob recipes saved");
    return ESP_OK;
}

esp_err_t load_recipe_list(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    printf("start of load recipe list\n");

    // Open
    err = nvs_open(BREWERY_SETUP_NAMESPACE, NVS_READONLY, &my_handle);
    if (err != ESP_OK)
    {
        printf("error err at top: %s\n", esp_err_to_name(err));
        return err;
    }

    //read string
    char *recipelist;

    size_t req_size = 0;

    err = nvs_get_str(my_handle, "recipelist", NULL, &req_size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
    {
        return err;
    }
    printf("error at start: %s\n", esp_err_to_name(err));
    if (req_size == 0)
    {
        // default value if not stored in nvs
        recipelist = malloc(strlen("{\"recipes\":[]}") + 1);
        strcpy(recipelist, "{\"recipes\":[]}");
    }
    else
    {
        printf("req_size after load: %d\n", req_size);
        recipelist = malloc(req_size);
        err = nvs_get_str(my_handle, "recipelist", recipelist, &req_size);
        if (err != ESP_OK)
        {
            printf("error: %s\n", esp_err_to_name(err));
            free(recipelist);
            return err;
        }
    }
    // update recipe list string
    update_recipe_list(recipelist);

    free(recipelist);
    printf("recipelist load success\n");

    // Close
    nvs_close(my_handle);
    return ESP_OK;
}

/* updates brewery setup struct */
void update_brewery_setup(int kettlevolume, int mashtunvolume, bool pumpedtransfer,
                          char *units, int leadtime, char *heatingmethod, char *coolingmethod)
{
    //add extra struct vars
    brewery_setup.main_config = 1;
    brewery_setup.external_connection = 0;
    brewery_setup.auto_fill = 1;
    brewery_setup.safety_margin = 5;

    brewery_setup.kettle_volume = kettlevolume;
    brewery_setup.mash_tun_volume = mashtunvolume;
    brewery_setup.pumped_transfer = pumpedtransfer;
    strcpy(brewery_setup.units, units);
    brewery_setup.lead_time = leadtime;
    strcpy(brewery_setup.heating_method, heatingmethod);
    strcpy(brewery_setup.cooling_method, coolingmethod);

    printf("brewery_setup updated - kettle: %d\n", brewery_setup.kettle_volume);
}

void update_recipe_list(char *recipelist)
{
    strcpy(esprecipelist, recipelist);
    //printf("esprecipelist updated: %s\n", esprecipelist);
}

char *get_esp_recipe_list(void)
{
    return esprecipelist;
}


// void Manual_Config(int valve1, int valve2, int valve3, int targetflow1, int targetflow2, int targetflow3, int pump, 
//     int heater, int heater_power, int temp_sensor, int target_temp, int time, int volume) {

// };




//void app_main(void)
void nvs_config(void)
{

    // update_brewery_setup();

    //update_brewery_setup(10,10);
    // printf("load brewery setup");
    // load_brewery_setup();

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    load_brewery_setup();
    load_recipe_list();

    // brew progress
    // Auto_Process = "auto process text";
    // Stage = "stage text";
    // Step = "step text";
    Status = 0; 

    BrewState = Passive_State; // 3
  
    
    Minutes_Remaining = 22;
    Seconds_Remaining = 44;

    // err = print_what_saved();
    // if (err != ESP_OK) printf("Error (%s) reading data from NVS!\n", esp_err_to_name(err));

    // err = save_restart_counter();
    // if (err != ESP_OK) printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));

    // gpio_reset_pin(BOOT_MODE_PIN);
    // gpio_set_direction(BOOT_MODE_PIN, GPIO_MODE_INPUT);

    /* Read the status of GPIO0. If GPIO0 is LOW for longer than 1000 ms,
       then save module's run time and restart it
     */
    // while (1) {
    //     if (gpio_get_level(BOOT_MODE_PIN) == 0) {
    //         vTaskDelay(1000 / portTICK_PERIOD_MS);
    //         if(gpio_get_level(BOOT_MODE_PIN) == 0) {
    //             err = save_run_time();
    //             if (err != ESP_OK) printf("Error (%s) saving run time blob to NVS!\n", esp_err_to_name(err));
    //             printf("Restarting...\n");
    //             fflush(stdout);
    //             esp_restart();
    //         }
    //     }
    //     vTaskDelay(200 / portTICK_PERIOD_MS);
    // }
}

// /**
// /* Non-Volatile Storage (NVS) Read and Write a Blob - Example

//    For other examples please check:
//    https://github.com/espressif/esp-idf/tree/master/examples

//    This example code is in the Public Domain (or CC0 licensed, at your option.)

//    Unless required by applicable law or agreed to in writing, this
//    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//    CONDITIONS OF ANY KIND, either express or implied.
// */
// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "nvs_flash.h"
// #include "nvs.h"
// #include "driver/gpio.h"

// #define STORAGE_NAMESPACE "storage"

// #if CONFIG_IDF_TARGET_ESP32C3
// #define BOOT_MODE_PIN GPIO_NUM_9
// #else
// #define BOOT_MODE_PIN GPIO_NUM_0
// #endif //CONFIG_IDF_TARGET_ESP32C3

// /* Save the number of module restarts in NVS
//    by first reading and then incrementing
//    the number that has been saved previously.
//    Return an error if anything goes wrong
//    during this process.
//  */
// esp_err_t save_restart_counter(void)
// {
//     nvs_handle_t my_handle;
//     esp_err_t err;

//     // Open
//     err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
//     if (err != ESP_OK) return err;

//     // Read
//     int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
//     err = nvs_get_i32(my_handle, "restart_conter", &restart_counter);
//     if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;

//     // Write
//     restart_counter++;
//     err = nvs_set_i32(my_handle, "restart_conter", restart_counter);
//     if (err != ESP_OK) return err;

//     // Commit written value.
//     // After setting any values, nvs_commit() must be called to ensure changes are written
//     // to flash storage. Implementations may write to storage at other times,
//     // but this is not guaranteed.
//     err = nvs_commit(my_handle);
//     if (err != ESP_OK) return err;

//     // Close
//     nvs_close(my_handle);
//     return ESP_OK;
// }

// /* Save new run time value in NVS
//    by first reading a table of previously saved values
//    and then adding the new value at the end of the table.
//    Return an error if anything goes wrong
//    during this process.
//  */
// esp_err_t save_run_time(void)
// {
//     nvs_handle_t my_handle;
//     esp_err_t err;

//     // Open
//     err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
//     if (err != ESP_OK) return err;

//     // Read the size of memory space required for blob
//     size_t required_size = 0;  // value will default to 0, if not set yet in NVS
//     err = nvs_get_blob(my_handle, "run_time", NULL, &required_size);
//     if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;

//     // Read previously saved blob if available
//     uint32_t* run_time = malloc(required_size + sizeof(uint32_t));
//     if (required_size > 0) {
//         err = nvs_get_blob(my_handle, "run_time", run_time, &required_size);
//         if (err != ESP_OK) {
//             free(run_time);
//             return err;
//         }
//     }

//     // Write value including previously saved blob if available
//     required_size += sizeof(uint32_t);
//     run_time[required_size / sizeof(uint32_t) - 1] = xTaskGetTickCount() * portTICK_PERIOD_MS;
//     err = nvs_set_blob(my_handle, "run_time", run_time, required_size);
//     free(run_time);

//     if (err != ESP_OK) return err;

//     // Commit
//     err = nvs_commit(my_handle);
//     if (err != ESP_OK) return err;

//     // Close
//     nvs_close(my_handle);
//     return ESP_OK;
// }

// /* Read from NVS and print restart counter
//    and the table with run times.
//    Return an error if anything goes wrong
//    during this process.
//  */
// esp_err_t print_what_saved(void)
// {
//     nvs_handle_t my_handle;
//     esp_err_t err;

//     // Open
//     err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
//     if (err != ESP_OK) return err;

//     // Read restart counter
//     int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
//     err = nvs_get_i32(my_handle, "restart_conter", &restart_counter);
//     if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
//     printf("Restart counter = %d\n", restart_counter);

//     // Read run time blob
//     size_t required_size = 0;  // value will default to 0, if not set yet in NVS
//     // obtain required memory space to store blob being read from NVS
//     err = nvs_get_blob(my_handle, "run_time", NULL, &required_size);
//     if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
//     printf("Run time:\n");
//     if (required_size == 0) {
//         printf("Nothing saved yet!\n");
//     } else {
//         uint32_t* run_time = malloc(required_size);
//         err = nvs_get_blob(my_handle, "run_time", run_time, &required_size);
//         if (err != ESP_OK) {
//             free(run_time);
//             return err;
//         }
//         for (int i = 0; i < required_size / sizeof(uint32_t); i++) {
//             printf("%d: %d\n", i + 1, run_time[i]);
//         }
//         free(run_time);
//     }

//     // Close
//     nvs_close(my_handle);
//     return ESP_OK;
// }

// void app_main(void)
// {
//     esp_err_t err = nvs_flash_init();
//     if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//         // NVS partition was truncated and needs to be erased
//         // Retry nvs_flash_init
//         ESP_ERROR_CHECK(nvs_flash_erase());
//         err = nvs_flash_init();
//     }
//     ESP_ERROR_CHECK( err );

//     err = print_what_saved();
//     if (err != ESP_OK) printf("Error (%s) reading data from NVS!\n", esp_err_to_name(err));

//     err = save_restart_counter();
//     if (err != ESP_OK) printf("Error (%s) saving restart counter to NVS!\n", esp_err_to_name(err));

//     gpio_reset_pin(BOOT_MODE_PIN);
//     gpio_set_direction(BOOT_MODE_PIN, GPIO_MODE_INPUT);

//     /* Read the status of GPIO0. If GPIO0 is LOW for longer than 1000 ms,
//        then save module's run time and restart it
//      */
//     while (1) {
//         if (gpio_get_level(BOOT_MODE_PIN) == 0) {
//             vTaskDelay(1000 / portTICK_PERIOD_MS);
//             if(gpio_get_level(BOOT_MODE_PIN) == 0) {
//                 err = save_run_time();
//                 if (err != ESP_OK) printf("Error (%s) saving run time blob to NVS!\n", esp_err_to_name(err));
//                 printf("Restarting...\n");
//                 fflush(stdout);
//                 esp_restart();
//             }
//         }
//         vTaskDelay(200 / portTICK_PERIOD_MS);
//     }
// }