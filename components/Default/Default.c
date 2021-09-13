#include <stdio.h>
#include "Default.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void Default (void)
{
    printf("Moving to default settings\n");
    Pause = 1;              //Pause set (Mainly for Heater PWM)
    Heater_Relay = 1;       //Heater off
    Pump_Relay = 1;         //Pump off
    Valve_1 = 0;
    vTaskDelay(5000 / portTICK_PERIOD_MS); //pause task for 5 seconds 
    printf("Valve_1 In Default Position\n");
    Valve_2 = 0;
    vTaskDelay(5000 / portTICK_PERIOD_MS); //pause task for 5 seconds
    printf("Valve_2 In Default Position\n");
    Valve_3 = 0;
    vTaskDelay(5000 / portTICK_PERIOD_MS); //pause task for 5 seconds
    printf("Valve_3 In Default Position\n");
    printf("In default settings\n");
    Defult_Setting = 1;         //Global variable of default state 1 true, 0 false
    vTaskDelete(NULL);          //Clear task
}