#include <stdio.h>
#include "HeaterPWM.h"
#include "HeaterPID.h"
#include "BrewStates.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define HEATER_PWM_PIN 25
#define PWM_Set_High (gpio_set_level(HEATER_PWM_PIN, 1))
#define PWM_Set_Low (gpio_set_level(HEATER_PWM_PIN, 0))

int Manual_Duty; //0-100 PWM value that is manually configured
int Heater_Duty_Cycle; //0-100 PWM value that is changed through Heater_PID function

void Heater_PWM (void)
{
    gpio_reset_pin(HEATER_PWM_PIN);
    gpio_set_direction(HEATER_PWM_PIN, GPIO_MODE_OUTPUT);
   
    TickType_t xLastWakeTime = xTaskGetTickCount(); //Saves LastWakeTime for use with vTaskDelayUntil

    while(PWM_En) 
    {

        if (Auto_PID == 1)      //Case to use PID not manual setting
        {
            Heater_Duty_Cycle = Heater_PID(Temp, Sensor);
        }

        else                       //Use manual duty cycle
        {
            Heater_Duty_Cycle = Manual_Duty;
        }
        
        /* Heater off (output low) */
        if (Heater_Duty_Cycle !=100)      //removes error if PWM = 100%
        {    
            //printf("Turning heater off\n");
            PWM_Set_Low;
            vTaskDelayUntil(&xLastWakeTime,(100-Heater_Duty_Cycle)); 
            //vTaskDelayUntil resumes task immediatly after specified time
        }

        /* Heater on (output high) */
        if (Heater_Duty_Cycle !=0)        //Removes error if PWM = 0%
        {
            //printf("Turning heater on\n");
            PWM_Set_High;
            vTaskDelayUntil(&xLastWakeTime, Heater_Duty_Cycle);
        }

    }
    
    PWM_Set_Low;
    vTaskDelete(NULL);          //Clear PWM task
}