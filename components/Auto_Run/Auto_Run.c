#include <stdio.h>
#include "Auto_Run.h"
#include "PumpRelay.h"
#include "HeaterRelay.h"
#include "HeaterPWM.h"
#include "HeaterPID.h"
#include "BrewStates.h"
#include "servo.h"
#include "interrupts.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"


void Auto_Run (struct Auto_Run_Controls *Auto_Run_Task)
{
    float Measured_Volume = 0;

    Absolute_Seconds_Remaining =-1;  //Initilised to non zero, prevents 0 minute hop additions being called immediately...
    
    HeaterRelay(Off);   //Turns off while moving valves for safety
    PumpRelay(Off);


    if ((Auto_Run_Task->Valve1 != Valve1_State) ||(Auto_Run_Task->FlowCtrl != Current_Flow1)) //if position or flow changed
    {
        //printf("Flow CTRL: %d\n", Auto_Run_Task->FlowCtrl); //test
        valve_tap_in.internal = Auto_Run_Task->Valve1; //Internal / external toggle
        Current_Flow1 = Auto_Run_Task->FlowCtrl;
        valve_set_position(Current_Flow1, &valve_tap_in); //Position defined by flow rate setting
    }

    vTaskDelay(200 / portTICK_PERIOD_MS);
    
    if ((Auto_Run_Task->Valve2 != Valve2_State)||(100 != Current_Flow2)) //if position or flow changed
    {
        valve_sparge_in.internal = Auto_Run_Task->Valve2; //Internal / external toggle
        Current_Flow2 = 100;
        valve_set_position(Current_Flow2, &valve_sparge_in); //Position defined by flow rate setting
    }
    
    if ((Auto_Run_Task->Valve3 != Valve3_State)||(100 != Current_Flow3)) //if position or flow changed
    {
        valve_sparge_out.internal = Auto_Run_Task->Valve3; //Internal / external toggle
        Current_Flow3 = 100;
        valve_set_position(Current_Flow3, &valve_sparge_out); //Position defined by flow rate setting
    }

    vTaskDelay(3000 / portTICK_PERIOD_MS); //pause for 2 seconds while valves move


    //Initilise temp pid
    PID_EN = 1;
    Temp = Auto_Run_Task->Target_Temp;             
    Sensor = Auto_Run_Task->Target_Sensor;         
    PumpRelay(Auto_Run_Task->Pump);
    HeaterRelay(Auto_Run_Task->Heater);

    
    if (Auto_Run_Task->Target_Temp !=0) 
    {
        strcpy (Auto_Process,"Heating to target temp");
        printf("*%s: %d*\n", Auto_Process, Temp);

        Auto_PID = 1;
        PWM_En = 1;
        xTaskCreate(
            Heater_PWM,                //function name
            "Heater PWM Control",      //function description
            2048,                      //stack size
            NULL,                      //task parameters
            1,                         //task priority
            NULL                      //task handle
        );

    
    
        while (!Temp_Reached) //Wait for temp to be reached
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
        }
    }

    //If a volume transefer is required, wait for completion
    if (Auto_Run_Task->Target_Volume != 0)
    {   
        strcpy (Auto_Process,"Transfering target volume");
        printf("*%s: %.2fL*\n", Auto_Process, Auto_Run_Task->Target_Volume);

        reset_meter_flow_total(&flowMeterTapIn);
        //Call volume function
        while (!Volume_Reached)
        {
            if (Auto_Run_Task->Target_Volume > (Measured_Volume/1000))
            {
                vTaskDelay(500 / portTICK_PERIOD_MS); //pause task for .5 second
                Measured_Volume = get_meter_flow_total(&flowMeterTapIn);
                printf("Transfered Volume: %.2fL\n",(Measured_Volume/1000));

                //Volume_Reached=1; //TEST VARIABLE!!!! remove in actual build
            }
            else
                Volume_Reached=1; 
        }
    }


    //Timer delay function
    if (Auto_Run_Task->Target_Time != 0)
    {
        strcpy (Auto_Process,"Waiting until target time");
        printf("*%s*\n", Auto_Process);

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

    HeaterRelay(Off);
    PumpRelay(Off);
    
    strcpy (Auto_Process,"");
    PID_EN = 0;
    Temp_Reached = 0;
    Volume_Reached = 0;
    Timer = 0;
    Absolute_Seconds_Remaining = 0;
    Minutes_Remaining = 0;
    Seconds_Remaining = 0;
    PWM_En = 0;             //Turn off heater PWM
    Stage_complete = 1;
    vTaskDelete(NULL);
}