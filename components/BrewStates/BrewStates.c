#include <stdio.h>
#include "BrewStates.h"
#include "Default.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//State machine setup variables
enum BrewStates {Passive_State, Test_State, WPS_State, Clean_State, Manual_State, Safety_Check_State, 
                  Mash_State, Sparge_State, Boil_State, Cooling_State, Transfer_State};
char BrewState;

int Manual_Duty = 20; //Manual duty cycle entry 0-100, 0==PID controlled, Global variable
int Pause = 0;       //Pause command global variable

//User input commands
int WPS_In, Clean_In, Manual_In, Pause_In, Reset_In, Brew_In;  // set to inerrupts?

int Defult_Setting;

void Brew_States (void)
{
     while(1)
   {
      switch (BrewState)
      {
         case Passive_State:
            //printf("Passive\n");
            Passive();
            break;

         case WPS_State:
            WPS();
            break;

         case Clean_State:
            Clean();
            break;

         case Manual_State:
            Manual();
            break;

         case Safety_Check_State:
            Safety_Check();
            break;

         case Mash_State:
            Mash();
            break;

         case Sparge_State:
            Sparge();
            break;

         case Boil_State:
            Boil();
            break;

         case Cooling_State:
            Cooling();
            break;

         case Transfer_State:
            Transfer();
            break;

         default:
            printf("Default\n");
            Passive();
         break;
      }
     
   }
}

void Passive (void)
{
   printf("Passive\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   if (!Defult_Setting)
   {
      xTaskCreate(
         Default,        //Call to default control function to zero everything
         "Reset valves, heater, pump and PWM", //function description
         1000,                      //stack size
         NULL,                      //task parameters
         2,                         //task priority
         NULL                       //task handle
      );      
      vTaskDelay(16000 / portTICK_PERIOD_MS); //pause task for 16 seconds
      Clean_In = 1; // test variable
   }
   
   else  //If Zeroed and safe wait for user input
   {
      if (WPS_In)
         BrewState = WPS_State;
      else if (Clean_In)
         BrewState = Clean_State;
      else if (Manual_In)
         BrewState = Manual_State;
      else if (Brew_In)
         BrewState = Safety_Check_State;
   }
}

void WPS (void)
{
   WPS_In = 0;             //Reset trigger variable
   printf("WPS\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Passive_State;
}

void Clean (void)
{
   Clean_In = 0;             //Reset trigger variable
   Defult_Setting = 0;     //Alert that config is no longer in default
   Manual_Duty = 50;    //test
   Pause = 0;           //test
   printf("Clean\n");
   vTaskDelay(10000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Passive_State;
}

void Manual (void)
{
   Manual_In = 0;             //Reset trigger variable
   Defult_Setting = 0;     //Alert that config is no longer in default
   printf("Manual\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Passive_State;
}

void Safety_Check (void)
{
   Brew_In = 0;             //Reset trigger variable
   printf("Safety_Check\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Mash_State;
}

void Mash (void)
{ 
   Defult_Setting = 0;  //Alert that config is no longer in default
   printf("Mash\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Sparge_State;
}

void Sparge (void)
{
   printf("Sparge\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Boil_State;
}

void Boil (void)
{
   printf("Boil\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Cooling_State;
}

void Cooling (void)
{
   printf("Cooling\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Transfer_State;
}

void Transfer (void)
{
   printf("Transfer\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = 132;
}