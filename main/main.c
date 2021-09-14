/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "esp_err.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_vfs_semihost.h"
#include "esp_vfs_fat.h"
#include "esp_spiffs.h"
#include "sdmmc_cmd.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mdns.h"
#include "lwip/apps/netbiosns.h"
#include <string.h>
#include <fcntl.h>
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "cJSON.h"



#include "protocol_examples_common.h"

#include "rest_server.h"
#include "esp_rest_main.h"

#include "led.h"
//#include "button.h"
#include "nvsblob.h"
#include "interrupts.h"


//for ds18b20
#include "string.h"
#include "ds18b20.h"

//For Heater Control
#include "HeaterPWM.h"


//For Primary State Machine
#include "BrewStates.h"


const int DS_PIN = 22;

//#include "pidpwm.h"
//#include "blink2.hpp"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
//#define BLINK_GPIO CONFIG_BLINK_GPIO

// struct Interrupts {
//         int gpio_num;
//         char message[50];
        
// }


//flowmeter etc
static uint32_t currentTime = 0;
static uint32_t previousTime = 0;
int timeInterval = 1000; // interval time in milli seconds


void getTempTask(void* arg) {

   int count = 0;

   while(1) {

   printf("%d - Temperature: %f\n", count, ds18b20_get_temp());
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   count++;
   }

}


void getFlowTask(void* args) {


while(1) {
   currentTime = esp_timer_get_time() / 1000ULL; // current time since boot in milliseconds
   //if (currentTime - previousTime > timeInterval) {

          //get flow rate, pass in time difference
          getFlowRate(currentTime - previousTime); //sends exact elapsed time to function


            previousTime = currentTime;

       //}

       vTaskDelay(1000 / portTICK_PERIOD_MS); //run roughly every 1 second

   }
}


void app_main(void)
{
    led_config();
    nvs_config();
    load_gpio_state(GPIO_LED);
    server_config();
   interrupts_config();

    struct Interrupts int1;
    strcpy(int1.message, "test message");
    int1.gpio_num = 55;

    button(&int1);

   

   xTaskCreate(
      Brew_States,              //function name
      "State Machine for Brew", //function description
      2048,                      //stack size
      NULL,                      //task parameters
      2,                         //task priority
      NULL                       //task handle
   );

   xTaskCreate(
      Heater_PWM,                //function name
      "Heater PWM Control",      //function description
      2048,                      //stack size
      NULL,               //task parameters
      1,                         //task priority
      NULL                       //task handle
   );
    
   //Heater_PWM(Manual_Duty);

    //ds18b20
    ds18b20_init(DS_PIN);

    //int count = 0;
 
    //flow rate
    flowM(); // add flow rate interrupt

  

//temp task
//xTaskCreate(getTempTask, "Temp task", 2048, NULL, 10, NULL );

//flow task
//xTaskCreate(getFlowTask, "flow task", 2048, NULL, 10, NULL );

   // while(1) {
      // led_toggle();
      //ds18b20
     // printf("%d - Temperature: %f\n", count, ds18b20_get_temp());
       //vTaskDelay(5000 / portTICK_PERIOD_MS); //ends up delaying flow rate, maybe make all of these into tasks. moved to 1 second loop
       //count++;



       //flow rate
      //  currentTime = esp_timer_get_time() / 1000ULL; // current time since boot in milliseconds
      //  if (currentTime - previousTime > timeInterval) {

      //     //get flow rate, pass in time difference
      //     getFlowRate(currentTime - previousTime);

      //     printf("%d - Temperature: %f\n", count, ds18b20_get_temp());
      //     count++;

      //       previousTime = currentTime;

      //  }

      //  vTaskDelay(100/ portTICK_PERIOD_MS); //has to have a delay in while 1 loop
    //}


}






//     /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
//        muxed to GPIO on reset already, but some default to other
//        functions and need to be switched to GPIO. Consult the
//        Technical Reference for a list of pads and their default
//        functions.)
//     */
//     gpio_reset_pin(BLINK_GPIO);
//     /* Set the GPIO as a push/pull output */
//     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);



//     led_config();

//     nvs_config();
//     load_gpio_state(BLINK_GPIO);
//     server_config();


//     //while(1) {
//         /* Blink off (output low) */
//         // printf("Turning off the LED\n");
//         // gpio_set_level(BLINK_GPIO, 0);
//         // vTaskDelay(1000 / portTICK_PERIOD_MS);
//         // /* Blink on (output high) */
//         // printf("Turning on the LED\n");
//         // gpio_set_level(BLINK_GPIO, 1);
//         // vTaskDelay(1000 / portTICK_PERIOD_MS);


//         //not blink stuff ( from other code)




//     //pass multiple args to function
//     struct Interrupts int1;
//     strcpy(int1.message, "test message");
//     int1.gpio_num = 55;

// interrupts_config();
//     //button(15); // this has to be first as it creates the scheduler
//     button(&int1);

//    //blink_config();
        


//    // }
//}
