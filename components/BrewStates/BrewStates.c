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

//User input config variables
int Main_Config = 2;    //1 = full, 2 = BIAB + Sparge, 3 = BIAB
int Kettle_Volume = 37; //0-127
int Mash_Tun_Volume = 60; //0-127
int Sparge_Connection = 1; //1 = tank, 0 = tap
int Cooling_Rqd = 1;  // 0 = n, 1 = y
int Cooling_Method = 0; // 0 = pumped water, 1 = pumped wort
int Transfer_Method = 1; //0 = manual, 1 = pumped
int Heating_Method = 1; //0 = boiler, 1 = RIMS
int Safety_Margin = 5; //xL of safety margin for kettle and mash tun (Hardcoded)

//User input recipe variables
float Grain_Weight_1 = 0.6;
float Grain_Weight_2 = 2;
float Grain_Weight_3 = 0;
float Grain_Weight_4 = 4;
float Grain_Weight_5 = 0;
float Grain_Weight_6 = 0;
float Grain_Weight_7 = 0;
float Grain_Weight_8 = 0;

float Mash_Water_Volume = 30;
float Sparge_Water_Volume = 30; 




float Total_Grain_Weight;
float Total_Brew_Volume;

int Kettle_Check;
int Mash_Check;

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
         1,                         //task priority
         NULL                       //task handle
      );      
      vTaskDelay(16000 / portTICK_PERIOD_MS); //pause task for 16 seconds
      Brew_In = 1; // test variable
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
   Total_Grain_Weight = (Grain_Weight_1 + Grain_Weight_2 + Grain_Weight_3 + Grain_Weight_4 + Grain_Weight_5 + Grain_Weight_6 + Grain_Weight_7 + Grain_Weight_8);
   Total_Brew_Volume = (Mash_Water_Volume + Sparge_Water_Volume - Total_Grain_Weight);
   Brew_In = 0;             //Reset trigger variable
   
   printf("Safety_Check\n");
   printf("Grain Weight: %f\n", Total_Grain_Weight);
   printf("Kettle Recipe Volume: %f\n", Total_Brew_Volume);
  
   Kettle_Check = (Total_Brew_Volume < (Kettle_Volume - Safety_Margin));

   if (Main_Config == 1)
   {
      Mash_Check = ((Total_Grain_Weight + Mash_Water_Volume) < (Mash_Tun_Volume - Safety_Margin));   
   }
   else
   {
      Mash_Check = ((Total_Grain_Weight + Mash_Water_Volume) < (Kettle_Volume - Safety_Margin));
   }

   if (Mash_Check && Kettle_Check)
   {
      BrewState = Mash_State;
   }
   else
      {
         if (!Kettle_Check)
         {
            printf("***WARNING*** Recipe exceeds kettle volume \n");
         }
         if (!Mash_Check)
         {
            printf("***WARNING*** Recipe exceeds Mash volume \n");
         }
         BrewState = Passive_State;
      }
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   
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