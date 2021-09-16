#include <stdio.h>
#include "Auto_Run.h"
#include "PumpRelay.h"
#include "HeaterRelay.h"
#include "HeaterPWM.h"
#include "HeaterPID.h"
#include "BrewStates.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void Auto_Run (int Valve1, int Valve2, int Valve3, int FlowCtrl, int Target_Temp, int Target_Sensor, int Pump, int Heater, int Target_Volume, int Target_Time)
{
    TickType_t xLastWakeTime = xTaskGetTickCount(); //Saves LastWakeTime for use with vTaskDelayUntil

    Stage_complete = 0;
    HeaterRelay(Off);   //Turns off while moving valves for safety, Maybe move to vlave function?
    PumpRelay(Off);
    //valve call, set all 3

    Valves_Set=1;

    while (!Valves_Set)      //Wait for valves to change
    {
       vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
    }


    //Set Flow valve to position 0-100

    //Initilise temp pid
    Auto_PID = 1;
    PWM_En = 1;
    Temp = Target_Temp;             //Assign global target temp
    Sensor = Target_Sensor;         //Assign global target sensor
    HeaterRelay(Heater);
    PumpRelay(Pump);
    xTaskCreate(
        Heater_PWM,                //function name
        "Heater PWM Control",      //function description
        2048,                      //stack size
        NULL,                      //task parameters
        1,                         //task priority
        NULL                       //task handle
    );

    //Wait for temp to be reached
    while (!Temp_Reached)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
    }

    //If a volume transefer is required, wait for completion
    if (Target_Volume != 0)
    {
        //Call volume function
        while (!Volume_Reached)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
        }
    }

    //Timer delay function
    if (Target_Time != 0)
    {
        while ((Target_Time*60) > Timer)    //conversion to seconds
        {
            vTaskDelayUntil(&xLastWakeTime, 100); //pause task for exactly 1 second

            Timer ++;           //Increment timer
            Absolute_Seconds_Remaining = ((Target_Time*60)-Timer);
            Minutes_Remaining = (((Target_Time*60)-Timer)/60);
            Seconds_Remaining = (Absolute_Seconds_Remaining-(Minutes_Remaining*60));
            printf("Time Remaining: %d:%d\n", Minutes_Remaining, Seconds_Remaining);
        }
    }

    Timer = 0;
    PWM_En = 0;             //Turn off heater PWM
    Stage_complete = 1;
}