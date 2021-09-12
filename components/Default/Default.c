#include <stdio.h>
#include "Default.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int Pause, Heater_Relay, Pump_Relay, Valve_1, Valve_2, Valve_3, Defult_Setting;

void Default (void)
{
    printf("Moving to default settings\n");
    Pause = 1;
    Heater_Relay = 0;
    Pump_Relay = 0;
    Valve_1 = 0;
    vTaskDelay(5000 / portTICK_PERIOD_MS); //pause task for 5 seconds 
    Valve_2 = 0;
    vTaskDelay(5000 / portTICK_PERIOD_MS); //pause task for 5 seconds
    Valve_3 = 0;
    vTaskDelay(5000 / portTICK_PERIOD_MS); //pause task for 5 seconds
    printf("In default settings\n");
    Defult_Setting = 1;         //Global variable of default state 1 true, 0 false
    vTaskDelete(NULL);          //Clear task
}