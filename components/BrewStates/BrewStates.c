#include <stdio.h>
#include "BrewStates.h"
#include "EquipConfig.h"
#include "ActiveRecipe.h"
#include "Default.h"
#include "PumpRelay.h"
#include "HeaterRelay.h"
#include "HeaterPWM.h"
#include "Auto_Run.h"
#include "Auto_Run_Setup.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void Auto_Run (struct Auto_Run_Controls *);

//State machine setup variables
enum BrewStates {Passive_State, Test_State, WPS_State, Clean_State, Manual_State, Safety_Check_State, 
                  Mash_State, Sparge_State, Boil_State, Cooling_State, Transfer_State};
char BrewState;

int Manual_Duty = 20; //Manual duty cycle entry 0-100, 0==PID controlled, Global variable

//User input commands
int WPS_In, Clean_In, Manual_In, Pause_In, Reset_In, Brew_In;  // set to inerrupts?

int Kettle_Check;
int Mash_Check;


void Brew_States (void)
{
     while(1)
   {
      switch (BrewState)
      {
         case Passive_State:
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
            BrewState = Passive_State;
         break;
      }
     
   }
}

void Passive (void)
{
   printf("Passive\n");
   EquipConfig();
   ActiveRecipe();
   Auto_Run_Setup();
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   if (!Defult_Setting && !Wait)
   {
      Wait = 1;        //Prevents looping back into task create
      xTaskCreate(
         Default,        //Call to default control function to zero everything
         "Reset valves, heater, pump and PWM", //function description
         2048,                      //stack size
         NULL,                      //task parameters
         1,                         //task priority
         NULL                       //task handle
      );
   }

   else if (Defult_Setting) //If Zeroed and safe wait for user input
   {
      Wait = 0;      //reset wait
      Brew_In = 1;   //test variable
      
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
   printf("Grain Weight: %f\n", Grain_Weight);
   printf("Kettle Recipe Volume: %f\n", Total_Brew_Volume);
  
   Kettle_Safe = (Kettle_Volume - Safety_Margin);
   Mash_Displacement = (Grain_Weight + Mash_Water_Volume);
   Mash_Safe = (Mash_Tun_Volume - Safety_Margin);

   Kettle_Check = (Total_Brew_Volume < Kettle_Safe);

   if (Main_Config == 1)
   {
      Mash_Check = (Mash_Displacement < Mash_Safe);
      Mash_Exceeded = (Mash_Displacement - Mash_Safe);
   }
   else
   {
      Mash_Check = (Mash_Displacement < Kettle_Safe);
      Mash_Exceeded = (Mash_Displacement - Kettle_Safe);
   }

   if (Mash_Check && Kettle_Check)
   {
      BrewState = Mash_State;
   }
   else
   {
      if (!Kettle_Check)
      {
         printf("***WARNING*** Recipe exceeds kettle volume by %f, L\n", (Total_Brew_Volume-Kettle_Safe));
      }
      if (!Mash_Check)
      {
         printf("***WARNING*** Recipe exceeds mash volume by %f, L\n", Mash_Exceeded);
      }
      BrewState = Passive_State;
   }
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   
}




void Mash (void)
{ 
   Defult_Setting = 0;  //Alert that config is no longer in default
   printf("Mash\n");

   Stage_complete = 0;
   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Strike_Heat,             //task parameters
      1,                         //task priority
      NULL                       //task handle
   );
   while (!Stage_complete)
   {
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   if (Main_Config != 1)   //BIAB
   {
      //****Wait for user to confirm bag is in place
   }

   Stage_complete = 0;
   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Mash1,                    //task parameters
      1,                         //task priority
      NULL                       //task handle
   );
   while (!Stage_complete)
   {
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   Stage_complete = 0;
   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Mash2,                    //task parameters
      1,                         //task priority
      NULL                       //task handle
   );
   while (!Stage_complete)
   {
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   Stage_complete = 0;
   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Mash3,                    //task parameters
      1,                         //task priority
      NULL                       //task handle
   );
   while (!Stage_complete)
   {
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   Stage_complete = 0;
   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Mash4,                    //task parameters
      1,                         //task priority
      NULL                       //task handle
   );
   while (!Stage_complete)
   {
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   Stage_complete = 0;
   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Mash5,                    //task parameters
      1,                         //task priority
      NULL                       //task handle
   );
   while (!Stage_complete)
   {
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   Stage_complete = 0;

   HeaterRelay(Off);
   PumpRelay(Off);

   //Mash Drain delay. pump off, Valve 3 open to drain mash tun delay until flowrate2 < xL...?

   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Sparge_State;
}



void Sparge (void)
{
   printf("Sparge\n");

   if ((Sparge_Water_Volume == 0)||(Main_Config == 3)) //Sparge not required
      BrewState = Boil_State; 

   else if (!Heating_Method) //Internal boiler
   {
      //wait for user prompt
      PumpRelay(On);
      //volume transfer function of pre-heated sparge water
      PumpRelay(Off);
      BrewState = Boil_State;
   }
   else
   {
      if (Main_Config == 1)
      {
         //Instant_Heat(); //valves set to external mash tun
      }
      else
      {
         //Instant_Heat(); //valves set to kettle, BIAB
      }

   }
   PumpRelay(On);
   //if (flow max and (temp > target temp))
         //Heater power --5;                 //ensures flow rate can maintain temp pid if water has been preheated
         //delay x 
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Boil_State;
}

void Boil (void)
{
   printf("Boil\n");

   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Boiling,                  //task parameters
      1,                         //task priority
      NULL                       //task handle
   );
   while (!Stage_complete)
   {
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }
     
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Cooling_State;
}

void Cooling (void)
{
   printf("Cooling\n");
   PumpRelay(Off);
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = Transfer_State;
}

void Transfer (void)
{
   printf("Transfer\n");
   PumpRelay(On);
   HeaterRelay(On);
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   BrewState = 132;        //default test
}