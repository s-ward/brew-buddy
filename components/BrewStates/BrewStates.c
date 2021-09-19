#include <stdio.h>
#include "string.h"
#include "BrewStates.h"
#include "EquipConfig.h"
#include "ActiveRecipe.h"
#include "PumpRelay.h"
#include "HeaterRelay.h"
#include "HeaterPWM.h"
#include "Auto_Run.h"
#include "Auto_Run_Setup.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t Auto_Task = NULL;
TaskHandle_t Passive_Task = NULL;
TaskHandle_t WPS_Task = NULL;
TaskHandle_t Cleaning_Task = NULL;
TaskHandle_t Manual_Task = NULL;
TaskHandle_t Mash_Task = NULL;
TaskHandle_t Sparge_Task = NULL;
TaskHandle_t Boil_Task = NULL;
TaskHandle_t Cooling_Task = NULL;
TaskHandle_t Transfer_Task = NULL;

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
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  Passive,                  //function name
                  "Passive",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &Passive_Task                  //task handle
               );
            }      
            break;
         }

         case WPS_State:
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  WPS,                  //function name
                  "WPS",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &WPS_Task                  //task handle
               );
            }      
            break;
         }

         case Clean_State:
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  Clean,                  //function name
                  "Clean",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &Cleaning_Task                  //task handle
               );
            }      
            break;
         }

         case Manual_State:
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  Manual,                  //function name
                  "Manual",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &Manual_Task                  //task handle
               );
            }      
            break;
         }

         case Safety_Check_State:
            Safety_Check();
            break;

         case Mash_State:
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  Mash,                  //function name
                  "Mash",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &Mash_Task                  //task handle
               );
            }      
            break;
         }

         case Sparge_State:
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  Sparge,                  //function name
                  "Sparge",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &Sparge_Task                  //task handle
               );
            }       
            break;
         }

         case Boil_State:
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  Boil,                  //function name
                  "boil",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &Boil_Task                  //task handle
               );
            }       
            break;
         }
         case Cooling_State:
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  Cooling,                  //function name
                  "Cooling",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &Cooling_Task                  //task handle
               );
            }       
            break;
         }

         case Transfer_State:
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  Transfer,                  //function name
                  "Transfer",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &Transfer_Task                  //task handle
               );
            }       
            break;
         }

         default:
            printf("Default\n");
            BrewState = Passive_State;
         break;
      }
      
      if ((Timer==10)||(Pause_In)) //Pause state (Replace if statement with pause command flag)
      {
         if (Stage_complete)
            Auto_Task = NULL;

         Pause_In = 1;
         Pump_State = Pump_Is_On;
         Heater_State = Heater_Is_On;
         Paused = 1;
         HeaterRelay(Off);
         PumpRelay(Off);

         if (Auto_Task != NULL)
            vTaskSuspend(Auto_Task);
         if (WPS_Task != NULL)
            vTaskSuspend(WPS_Task);
         if (Cleaning_Task != NULL)
            vTaskSuspend(Cleaning_Task);
         if (Manual_Task != NULL)
            vTaskSuspend(Manual_Task);
         if (Mash_Task != NULL)
            vTaskSuspend(Mash_Task);
         if (Sparge_Task != NULL)
            vTaskSuspend(Sparge_Task);
         if (Boil_Task != NULL)
            vTaskSuspend(Boil_Task);
         if (Cooling_Task != NULL)
            vTaskSuspend(Cooling_Task);
         if (Transfer_Task != NULL)
            vTaskSuspend(Transfer_Task);

         printf("Paused\n");
         vTaskDelay(5000 / portTICK_PERIOD_MS); //pause task for 5 seconds, 
         Pause_In = 0;     //Testbench setting

         if (!Pause_In)
         {
            Paused = 0;
            printf("Resumed\n");
            
            if (WPS_Task != NULL)
               vTaskResume(WPS_Task);
            if (Cleaning_Task != NULL)
               vTaskResume(Cleaning_Task);
            if (Manual_Task != NULL)
               vTaskResume(Manual_Task);
            if (Mash_Task != NULL)
               vTaskResume(Mash_Task);
            if (Sparge_Task != NULL)
               vTaskResume(Sparge_Task);
            if (Boil_Task != NULL)
               vTaskResume(Boil_Task);
            if (Cooling_Task != NULL)
               vTaskResume(Cooling_Task);
            if (Transfer_Task != NULL)
               vTaskResume(Transfer_Task);
            if (Auto_Task != NULL)
               vTaskResume(Auto_Task);

            PumpRelay(Pump_State);
            HeaterRelay(Heater_State);
         }
      }
     vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }
}

void Passive (void)
{
   printf("Passive\n");
   EquipConfig();
   ActiveRecipe();
   Auto_Run_Setup();
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second

//Zeroise all components
   Stage_complete = 0;
   xTaskCreate(   
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Zeroise,                    //task parameters
      1,                         //task priority
      &Auto_Task                       //task handle
   );
   while (!Stage_complete)
   {
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   Stage_complete = 0;
   Brew_In = 1;   //test variable
   
   if (WPS_In)
   {
      Step_Active = 0;
      BrewState = WPS_State;
      vTaskDelete(NULL);
   }
   else if (Clean_In)
   {
      Step_Active = 0;
      BrewState = Clean_State;
      vTaskDelete(NULL);
   }
   else if (Manual_In)
   {
      Step_Active = 0;
      BrewState = Manual_State;
      vTaskDelete(NULL);
   }
   else if (Brew_In)
   {
      Step_Active = 0;
      BrewState = Safety_Check_State;
      vTaskDelete(NULL);
   }
}

void WPS (void)
{
   WPS_In = 0;             //Reset trigger variable
   printf("WPS\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   Step_Active = 0;
   BrewState = Passive_State;
   vTaskDelete(NULL);
}

void Clean (void)
{
   Clean_In = 0;             //Reset trigger variable
   printf("Clean\n");
   vTaskDelay(10000 / portTICK_PERIOD_MS); //pause task for 1 second
   Step_Active = 0;
   BrewState = Passive_State;
   vTaskDelete(NULL);
}

void Manual (void)
{
   Manual_In = 0;             //Reset trigger variable
   printf("Manual\n");
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   Step_Active = 0;
   BrewState = Passive_State;
   vTaskDelete(NULL);
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
}




void Mash (void)
{ 
   printf("Mash\n");

   Stage_complete = 0;
   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Strike_Heat,             //task parameters
      1,                         //task priority
      &Auto_Task                       //task handle
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
      &Auto_Task                       //task handle
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
      &Auto_Task                       //task handle
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
      &Auto_Task                       //task handle
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
      &Auto_Task                       //task handle
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
      &Auto_Task                       //task handle
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
   Step_Active = 0;
   BrewState = Sparge_State;
   vTaskDelete(NULL);
}



void Sparge (void)
{
   printf("Sparge\n");

   if ((Sparge_Water_Volume == 0)||(Main_Config == 3)) //Sparge not required
   {
      Step_Active = 0;
      BrewState = Boil_State;
      vTaskDelete(NULL); 
   }

   else if (!Heating_Method) //Internal boiler
   {
      //wait for user prompt
      PumpRelay(On);
      //volume transfer function of pre-heated sparge water
      PumpRelay(Off);
      Step_Active = 0;
      BrewState = Boil_State;
      vTaskDelete(NULL);
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
   Step_Active = 0;
   BrewState = Boil_State;
   vTaskDelete(NULL);
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
      &Auto_Task                  //task handle
   );
      

   while (!Stage_complete)
   {  
      if (Count_Update)
      {   
         if ((Absolute_Seconds_Remaining == Adjunct_Time_1*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_1 == 0)&&(strcmp (Adjunct_Name_1,""))))
            printf("Please add %s\n", Adjunct_Name_1);
         if ((Absolute_Seconds_Remaining == Adjunct_Time_2*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_2 == 0)&&(strcmp (Adjunct_Name_2,""))))
            printf("Please add %s\n", Adjunct_Name_2);
         if ((Absolute_Seconds_Remaining == Adjunct_Time_3*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_3 == 0)&&(strcmp (Adjunct_Name_3,""))))
            printf("Please add %s\n", Adjunct_Name_3);
         if ((Absolute_Seconds_Remaining == Adjunct_Time_4*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_4 == 0)&&(strcmp (Adjunct_Name_4,""))))
            printf("Please add %s\n", Adjunct_Name_4);
         if ((Absolute_Seconds_Remaining == Adjunct_Time_5*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_5 == 0)&&(strcmp (Adjunct_Name_5,""))))
            printf("Please add %s\n", Adjunct_Name_5);
         if ((Absolute_Seconds_Remaining == Adjunct_Time_6*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_6 == 0)&&(strcmp (Adjunct_Name_6,""))))
            printf("Please add %s\n", Adjunct_Name_6);
         if ((Absolute_Seconds_Remaining == Adjunct_Time_7*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_7 == 0)&&(strcmp (Adjunct_Name_7,""))))
            printf("Please add %s\n", Adjunct_Name_7);
         if ((Absolute_Seconds_Remaining == Adjunct_Time_8*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_8 == 0)&&(strcmp (Adjunct_Name_8,""))))
            printf("Please add %s\n", Adjunct_Name_8);
         Count_Update=0;

         if ((Absolute_Seconds_Remaining == 300)&& Cooling_Rqd) //5 minutes remaining, Prompts and pause to connect cooler
            {
               if (Main_Config == 1)
               {
                  printf("Please disconnect mash tun and connect cooler\n");
               }
               else if (Cooling_Method)   //BIAB and Pumped wort
               {
                  printf("Please connect cooler to kettle return line\n");
               }
               else     ///BIAB pumped water
               {
                  printf("Please connect cooler to external source input\n");
               }
               if (!Cooling_Method)    //Pumped water, sterilise kettle
                  printf("Please immerse cooler in kettle\n");
               Pause_In = 1;
               if (Cooling_Method)
               {
                  //Valve function call(Valve2 and 3 to "Mash Tun") Once resuemed, if wort recirculation
                  //Maybe remove these for "Wort recirculation" as wort will be boiling when pumped 
                  //Amd pumping at T-5 will cause loss of boil temp
                  //all needs a revamp actually as will be pumping through rims
               }
            }

         //Prompt to add cooler
      }

      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 seconds
   }


   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   Step_Active = 0;
   BrewState = Cooling_State;
   vTaskDelete(NULL);
}

void Cooling (void)
{
   printf("Cooling\n");
   if (Cooling_Rqd)
   {
      
      printf("Ensure cooler is connected and positioned correctly\n");
      Auto_Task = NULL;
      Pause_In = 1;  //Wait for user confirmation
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
      xTaskCreate(
         Auto_Run,                  //function name
         "Auto Control",            //function description
         2048,                      //stack size
         &Cool,                  //task parameters
         1,                         //task priority
         &Auto_Task                  //task handle
      );
      
      while (Cool.Target_Sensor != Cooling_Temp)
      { 
         vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
      }
      PumpRelay(Off);
   }
   //Check for pumped Wort and transfer, if true carry our sanitise function
   Step_Active = 0;
   BrewState = Transfer_State;
   vTaskDelete(NULL);
}

void Transfer (void)
{
   printf("Transfer\n");
   PumpRelay(On);
   HeaterRelay(On);
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   Step_Active = 0;
   BrewState = 132;        //default test
   vTaskDelete(NULL);
}