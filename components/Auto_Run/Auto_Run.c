#include <stdio.h>
#include "Auto_Run.h"
#include "PumpRelay.h"
#include "HeaterRelay.h"
#include "HeaterPWM.h"
#include "HeaterPID.h"
#include "BrewStates.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//void Auto_Run (struct Auto_Run_Controls);
// int Target_Temp;

void Auto_Run (struct Auto_Run_Controls *Auto_Run_Task)
{
    Absolute_Seconds_Remaining =-1;  //Initilised to non zero, prevents 0 minute hop additions being called immediately...
 
    //TickType_t xLastWakeTime = xTaskGetTickCount(); //Saves LastWakeTime for use with vTaskDelayUntil
    
    HeaterRelay(Off);   //Turns off while moving valves for safety, Maybe move to vlave function?
    PumpRelay(Off);
    //valve call, set all 3(Valve1, Valve2, Valve3)

    Valves_Set=1;

    while (!Valves_Set)      //Wait for valves to change
    {
       vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
    }


    //Set Flow valve to position (FlowCtrl)

    //Initilise temp pid
    Auto_PID = 1;
    PWM_En = 1;
    Temp = Auto_Run_Task->Target_Temp;             //Assign global target temp
    Sensor = Auto_Run_Task->Target_Sensor;         //Assign global target sensor
    PumpRelay(Auto_Run_Task->Pump);
    HeaterRelay(Auto_Run_Task->Heater);
    xTaskCreate(
        Heater_PWM,                //function name
        "Heater PWM Control",      //function description
        2048,                      //stack size
        NULL,                      //task parameters
        1,                         //task priority
        NULL                      //task handle
    );

    if (Auto_Run_Task->Target_Temp !=0)         //Check to make default call work (zeroed I/P's)
    {
    
        while (!Temp_Reached) //Wait for temp to be reached
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
        }
    }

    //If a volume transefer is required, wait for completion
    if (Auto_Run_Task->Target_Volume != 0)
    {
        //Call volume function
        while (!Volume_Reached)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
            Volume_Reached=1;  //test
        }
    }

    Volume_Reached=0; //Test

    //Timer delay function
    if (Auto_Run_Task->Target_Time != 0)
    {
        while ((Auto_Run_Task->Target_Time*60) > Timer)    //conversion to seconds
        {
            TickType_t xLastWakeTime = xTaskGetTickCount(); //Saves LastWakeTime for use with vTaskDelayUntil

            Absolute_Seconds_Remaining = ((Auto_Run_Task->Target_Time*60)-Timer);
            Minutes_Remaining = (((Auto_Run_Task->Target_Time*60)-Timer)/60);
            Seconds_Remaining = (Absolute_Seconds_Remaining-(Minutes_Remaining*60));
            printf("Time Remaining: %d:%d\n", Minutes_Remaining, Seconds_Remaining);
            
            Count_Update = 1;       //Fix for hop appition call happening twice

            vTaskDelayUntil(&xLastWakeTime, 100); //pause task for exactly 1 second

            Timer ++;           //Increment timer

            
            
            
        }
    }

    Temp_Reached = 0;
    Volume_Reached = 0;
    Valves_Set = 0;
    Timer = 0;
    PWM_En = 0;             //Turn off heater PWM
    Stage_complete = 1;
    vTaskDelete(NULL);
}