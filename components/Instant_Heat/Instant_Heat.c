#include <stdio.h>
#include "string.h"
#include "Instant_Heat.h"
#include "BrewStates.h"
#include "HeaterPWM.h"
#include "servo.h"
#include "PumpRelay.h"
#include "HeaterRelay.h"
#include "EquipConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void Instant_Heat (struct Instant_Heat_Controls *Temp_Flow)
{
    Wait = 1;

    if (External_Connection && (BrewState != Manual_State))
        PumpRelay(Off);
    else
        PumpRelay(On);

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
            printf("*%s*\n", Auto_Process);

            //Call volume function
            while (!Volume_Reached)
            {
                vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
                Volume_Reached=1;  //test
            }
            Wait = 0;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
    }

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
    int Sensor2 = 50;

    while(FlowPID_En)
    {
        if (PID_Flow != Current_Flow1)
        {
            Current_Flow1 = PID_Flow;
            valve_set_position(Current_Flow1, &valve_tap_in);   //Only using valve 1
        }
        //PID fnk

        //Safety check for unable to maintain temp with flow rate alone
        if (ReCheck != 0)
            ReCheck = ReCheck - 1;
        if(PID_Flow == 100 &&(Sensor2 > Targ_Temp) && (!ReCheck))   //if flow max and temp too great
        {    
            Manual_Duty = (Manual_Duty-5);  //Reduce heater power by 5%
            ReCheck = 50;                   //Wait 5 seconds before checking again
        }        
        vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
    }
    vTaskDelete(NULL);
}