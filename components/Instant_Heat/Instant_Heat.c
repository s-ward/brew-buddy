#include <stdio.h>
#include "string.h"
#include "Instant_Heat.h"
#include "BrewStates.h"
#include "HeaterPWM.h"
#include "servo.h"
#include "PumpRelay.h"
#include "HeaterRelay.h"
#include "EquipConfig.h"
#include "HeaterPID.h"
#include "interrupts.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void Instant_Heat (struct Instant_Heat_Controls *Temp_Flow)
{
    float Measured_Volume = 0;
    Wait = 1;

    PID_EN = 1;

    if (BrewState != Manual_State)
    {
        if (External_Connection)
            PumpRelay(Off);
        else
            PumpRelay(On);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS); //Sometimes heater would not initilise due to Pump != On yet, delay solves issue

    //Initilise Heater for manual mode
    HeaterRelay(On);
    PWM_En = 1;
    Auto_PID = 0;
    Manual_Duty = 100;  
    xTaskCreate(
        Heater_PWM,                //function name
        "Heater PWM Control",      //function description
        2048,                      //stack size
        NULL,                      //task parameters
        1,                         //task priority
        NULL                      //task handle
    );

    //Initilise flowPID
    FlowPID_En = 1;
    xTaskCreate(
        FlowPID,                //function name
        "Flow PID Control",      //function description
        2048,                      //stack size
        Temp_Flow->Instant_Temp,   //task parameters
        1,                         //task priority
        NULL                      //task handle
    );

    while (Wait)
    {
        if (Temp_Flow->Instant_Volume != 0)
        {   
            strcpy (Auto_Process,"Transfering target volume");
            printf("*%s: %.2fL*\n", Auto_Process, Temp_Flow->Instant_Volume);

            reset_meter_flow_total(&flowMeterTapIn);
            //Call volume function
            while (!Volume_Reached)
            {
                if (Temp_Flow->Instant_Volume > (Measured_Volume/1000))
                {
                    vTaskDelay(500 / portTICK_PERIOD_MS); //pause task for .5 second
                    Measured_Volume = get_meter_flow_total(&flowMeterTapIn);
                    printf("Transfered Volume: %.2fL\n",(Measured_Volume/1000));

                    Volume_Reached=1; //TEST VARIABLE!!!! remove in actual build
                }
                else
                    Volume_Reached=1;
            }
            Wait = 0;
        }
        else
        {
            strcpy (Auto_Process,"Maintaining instant heat temp");
            printf("*%s*\n", Auto_Process);

            while (ManState == Manual_Instant_Heat)    //Indefinite instant heat 
            {
                vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
            }
            Wait = 0;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
    }   

    PID_EN = 0;
    FlowPID_En = 0;
    PWM_En = 0;
    Volume_Reached = 0;

    HeaterRelay(Off);
    PumpRelay(Off);

    Stage_complete = 1;
    vTaskDelete(NULL);    
}


void FlowPID(int Targ_Temp)
{
    int PID_Flow = 30;      //initilised to 30%
    int ReCheck = 0;

    while(FlowPID_En)
    {
        if (!Paused)
        {
            if (BrewState == Manual_State)
            {
                valve_tap_in.internal = Man_Valve1;
            }
            if (PID_Flow != Current_Flow1)
            {
                Current_Flow1 = PID_Flow;
                valve_set_position(Current_Flow1, &valve_tap_in);   //Only using valve 1
            }
            
            PID_Flow = (100 - Heater_PID(Targ_Temp, 4)); //Calls PID function, 4 = Flow PID setting

            //Safety check for unable to maintain temp with flow rate alone
            if (ReCheck != 0)
                ReCheck = ReCheck - 1;
            if(PID_Flow == 100 &&(Temp2 > Targ_Temp) && (!ReCheck))   //if flow max and temp too great
            {    
                Manual_Duty = (Manual_Duty-5);  //Reduce heater power by 5%
                ReCheck = 50;                   //Wait 5 seconds before checking again
            }        
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
    }
    vTaskDelete(NULL);
}