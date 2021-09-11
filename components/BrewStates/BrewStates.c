#include <stdio.h>
#include "BrewStates.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum BrewStates {Passive_State, Test_State, WPS_State, Clean_State, Manual_State, Safety_Check_State, 
                  Mash_State, Sparge_State, Boil_State, Cooling_State, Transfer_State};
char BrewState;

int Manual_Duty = 20; //Manual duty cycle entry 0-100, 0==PID controlled, Global variable
int Pause = 0;       //Pause command global variable

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
   Pause = 0;  
   Manual_Duty = 50;
   printf("Passive\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = WPS_State;
}

void WPS (void)
{
   printf("WPS\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Clean_State;
}

void Clean (void)
{
   printf("Clean\n");
   Manual_Duty = 99;
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Manual_State;
}

void Manual (void)
{
   printf("Manual\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Safety_Check_State;
}

void Safety_Check (void)
{
   printf("Safety_Check\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Mash_State;
}

void Mash (void)
{ 
   printf("Mash\n");
   Manual_Duty = 1;
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
   Pause = 1;
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