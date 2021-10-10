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
#include "HeaterPID.h"


//For Primary State Machine
#include "BrewStates.h"

#include "servo.h"

const int T1_PIN = 22;
const int T2_PIN = 19;
const int T3_PIN = 5;


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

void PIDTestTask(void)
{
   int DutyTest = 0;

   while (1)
   {
      DutyTest = Heater_PID (30, 1);
      printf("Duty Cycle: %d\n", DutyTest);
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }
}

void getTempTask(void *arg)
{
   int count = 0;

   while (1)
   {
      ds18b20_init(T1_PIN);
      Temp1 = ds18b20_get_temp();
      printf("%d - Sensor 1 - Temperature: %f\n", count, Temp1);
      ds18b20_init(T2_PIN);
      Temp2 = ds18b20_get_temp();
      printf("%d - Sensor 2 - Temperature: %f\n", count, Temp2);
      ds18b20_init(T3_PIN);
      Temp3 = ds18b20_get_temp();
      printf("%d - Sensor 3 - Temperature: %f\n", count, Temp3);
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
      count++;
   }
}

void getFlowTask(void *args)
{

   while (1)
   {
      currentTime = esp_timer_get_time() / 1000ULL; // current time since boot in milliseconds
                                                    //if (currentTime - previousTime > timeInterval) {

      //get flow rate, pass in time difference
      getFlowRate(currentTime - previousTime); //sends exact elapsed time to function

      previousTime = currentTime;

      //}

      vTaskDelay(1000 / portTICK_PERIOD_MS); //run roughly every 1 second
   }
}

void updateFlowMeterTask(void *args)
{
   while (1)
   {
      struct FlowMeter *flowMeter = args;

      updateFlowMeter(flowMeter);
      
      vTaskDelay(1000 / portTICK_PERIOD_MS); //run roughly every 1 second
     // printf("Flow Meter GPIO: %d, Flow Rate: %d ml/m, Flow Total: % d, Pulse Count Total: % d, Pointer Address: %p\n", 
       //  flowMeter->gpio_num, flowMeter->flow_rate, flowMeter->flow_total, flowMeter->pulse_count_total, flowMeter);
   }
}

void valve_generic_example_task(void *arg)
{

   servo_params *valve = arg;

   while (1)
   {
     // printf("%s GPIO Number: %d, Pointer Address: %p\n", valve->name, valve->gpio_num, valve);
      // printf("%s Pointer Address: %p\n", valve->name, valve);

      valve_set_position(VALVE_CLOSE, valve);
      vTaskDelay(((esp_random() % 1000) + 1000) / portTICK_PERIOD_MS);
      //printf("%s Position: %d\n", valve->name, valve_get_position(valve));

      valve_set_position(170, valve);
      vTaskDelay(((esp_random() % 1000) + 1000) / portTICK_PERIOD_MS);
      //printf("%s Position: %d\n", valve->name, valve_get_position(valve));
   }
}

void timertesttask(void)
{
   while (1)
   {
      
     Seconds_Remaining--;
     if (Seconds_Remaining == 0) {
         Minutes_Remaining--;
         Seconds_Remaining = 60;
     }
      
      vTaskDelay(1000 / portTICK_PERIOD_MS); //run roughly every 1 second

   }
}

void app_main(void)
{
   PIDController_Init();
   server_config();

   //led_config();
   // load_gpio_state(GPIO_LED);
   nvs_config();

   //struct Interrupts int1;
   //strcpy(int1.message, "test message");
   //int1.gpio_num = 55;

   //button(&int1);

   //ds18b20
   //ds18b20_init(DS_PIN);

   //int count = 0;
   //valve_tap_in.gpio_num = 17;
   //update_servo_params(&valve_tap_in, 17 );
   // update_servo_params();

   servo_init();
   interrupts_config();

   //valve example tasks
   //xTaskCreate(valve_generic_example_task, "valve 2 task", 2048, &valve_sparge_in, 10, NULL);
   //xTaskCreate(valve_generic_example_task, "valve 2 task", 2048, &valve_sparge_out, 10, NULL);
   //xTaskCreate(valve_generic_example_task, "valve 2 task", 2048, &valve_tap_in, 10, NULL);

   //flow rate
   //flowM(); // add flow rate interrupt
   //xTaskCreate(timertesttask, "Timer Test", 2048, NULL, 10, NULL);
   //temp task
   xTaskCreate(getTempTask, "Temp task", 2048, NULL, 10, NULL );

   xTaskCreate(PIDTestTask, "PID task", 2048, NULL, 10, NULL );
   // xTaskCreate(valve_generic_example_task, "valve 2 task", 2048, &valve_sparge_in, 10, NULL);
   // xTaskCreate(valve_generic_example_task, "valve 2 task", 2048, &valve_sparge_out, 10, NULL);
   // xTaskCreate(valve_generic_example_task, "valve 2 task", 2048, &valve_tap_in, 10, NULL);



   

   xTaskCreate(
      Brew_States,              //function name
      "State Machine for Brew", //function description
      2048,                      //stack size
      NULL,                      //task parameters
      2,                         //task priority
      NULL                       //task handle
   );
    

    //ds18b20

    //int count = 0;
 

    //flow rate
    flowM(); // add flow rate interrupt
    getFlowRate(1000);

   //flow task
   //xTaskCreate(getFlowTask, "flow task", 2048, NULL, 10, NULL );
   xTaskCreate(updateFlowMeterTask, "Update Flow Meter Task", 2048, &flowMeterTapIn, 10, NULL);
   xTaskCreate(updateFlowMeterTask, "Update Flow Meter Task", 2048, &flowMeterSpargeIn, 10, NULL);

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
