#include <stdio.h>
#include "HeaterPWM.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define HEATER_PWM_PIN 25

int Heater_Duty_Cycle; //0-100 PWM value, global variable that is changed through Heater_PID function

void Heater_PWM (void)
{
    gpio_reset_pin(HEATER_PWM_PIN);
    gpio_set_direction(HEATER_PWM_PIN, GPIO_MODE_OUTPUT);
   
    TickType_t xLastWakeTime = xTaskGetTickCount(); //Saves LastWakeTime for use with vTaskDelayUntil

    while(1) 
    {
        /* Heater off (output low) */
        if (Heater_Duty_Cycle !=100)      //removes error if PWM = 100%
        {    
            printf("Turning heater off\n");
            gpio_set_level(HEATER_PWM_PIN, 0);
            vTaskDelayUntil(&xLastWakeTime,(100-Heater_Duty_Cycle)); 
            /*vTaskDelayUntil resumes task immediatly after specified time
            vTaskDelay is not sufficient to guarantee a stable frequency*/
        }

        /* Heater on (output high) */
        if (Heater_Duty_Cycle !=0)        //Removes error if PWM = 0%
        {
            printf("Turning heater on\n");
            gpio_set_level(HEATER_PWM_PIN, 1);
            vTaskDelayUntil(&xLastWakeTime, Heater_Duty_Cycle);
        }
    }
}