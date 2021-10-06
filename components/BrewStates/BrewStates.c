#include <stdio.h>
#include "string.h"
#include "BrewStates.h"
#include "EquipConfig.h"
#include "ActiveRecipe.h"
#include "PumpRelay.h"
#include "HeaterRelay.h"
#include "HeaterPWM.h"
#include "HeaterPID.h"
#include "Auto_Run.h"
#include "Auto_Run_Setup.h"
#include "servo.h"
#include "Instant_Heat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

TaskHandle_t Auto_Task = NULL;
TaskHandle_t Passive_Task = NULL;
TaskHandle_t WPS_Task = NULL;
TaskHandle_t Cleaning_Task = NULL;
TaskHandle_t Manual_Task = NULL;
TaskHandle_t Safety_Task = NULL;
TaskHandle_t Mash_Task = NULL;
TaskHandle_t Sparge_Task = NULL;
TaskHandle_t Boil_Task = NULL;
TaskHandle_t Cooling_Task = NULL;
TaskHandle_t Transfer_Task = NULL;
TaskHandle_t Pause_Task = NULL;
TaskHandle_t Instant_Heat_Task = NULL;

//User input commands
int WPS_In, Clean_In, Manual_In, Pause_In, Cancel_In, Brew_In;  // set to inerrupts?

int Kettle_Check;
int Mash_Check;


//Test variable for pause functionality
int Pause_Button;
int Mash_High;
#define TEST_PAUSE_PIN 32
#define TEST_CANCEL_PIN 33
#define Float1 35


void Brew_States (void)
{
   gpio_set_direction(TEST_PAUSE_PIN, GPIO_MODE_INPUT);
   gpio_set_direction(TEST_CANCEL_PIN, GPIO_MODE_INPUT);
   gpio_set_direction(Float1, GPIO_MODE_INPUT);

   while(1)
   {
   Pause_Button = gpio_get_level (TEST_PAUSE_PIN);
   Cancel_In = gpio_get_level (TEST_CANCEL_PIN);
   Mash_High = gpio_get_level (Float1);

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
         {
            if (!Step_Active)
            {
               Step_Active = 1;
               xTaskCreate(
                  Safety_Check,                  //function name
                  "Safety_Check",            //function description
                  2048,                      //stack size
                  NULL,                  //task parameters
                  1,                         //task priority
                  &Safety_Task                  //task handle
               );
            }
            break;
         }

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

      //Cancel pressed
      if (Cancel_In)
      {
         xTaskCreate(  
            Cancel,                  //function name
            "Cancel",            //function description
            2048,                      //stack size
            NULL,                    //task parameters
            1,                         //task priority
            NULL                       //task handle
         );
         vTaskDelay(2000 / portTICK_PERIOD_MS); // only required for manual testing with switch
         Cancel_In = 0;    //reset flag   
      }
      
      if (!Mash_High)   //Safety for mash tun overfill
      {
         strcpy (User_Int_Required,"***WARNING*** Mash tun is too full, please check for stuck sparge");
         User_Int_Rqd = 1; //Flag that interaction is required
         printf("%s\n", User_Int_Required);
         Pause_In = 1;
      }

      //Pause Pressed
      if (Pause_Button || Pause_In) //Pause state (Replace if statement with pause command flag)
      {
         if (!Paused)
         {
            Paused = 1;
            xTaskCreate(  
               Pause,                  //function name
               "Pause",            //function description
               2048,                      //stack size
               NULL,                    //task parameters
               1,                         //task priority
               &Pause_Task                       //task handle
            );
         }
      }

      if (Stage_complete)  // Required for pause
      {
         Auto_Task = NULL;
         Instant_Heat_Task = NULL;
      }

     vTaskDelay(100 / portTICK_PERIOD_MS); //Check for updated user input of embedded pause call every .1 seconds
   }
}

void Passive (void)
{
   int Safe = 0;

   //Reset variables
   Man_EN = 0;
   Paused = 0;
   FlowPID_En = 0;
   Pause_In = 0;
   Temp_Reached = 0;
   Volume_Reached = 0;
   Timer = 0;
   Absolute_Seconds_Remaining = 0;
   Minutes_Remaining = 0;
   Seconds_Remaining = 0;
   PWM_En = 0;
   Stage_complete = 0;
   User_Int_Rqd = 0;
   strcpy (Auto_Process,"");
   strcpy (User_Int_Required,"");
   strcpy (User_Adjunct_Required,"");

   strcpy (Stage,"Passive");
   strcpy (Step,"Awaiting user input");
   printf("-----%s: %s-----\n", Stage, Step);

   while(BrewState == Passive_State)
   {
      EquipConfig();       //will update while in passive if user changes config / recipe
      ActiveRecipe();      // Will move to Sams function call
      Auto_Run_Setup();    //Assign equip config to auto_run struct for use with clean and manual modes
      
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 seconds to assign vars

      //Zeroise all components
      if (!Safe)
      {
         printf("Zeroising\n");
         strcpy (Auto_Process,"Zeroising");
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
            vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
         }
         Safe = 1;
      }
      
      //Manual_In = 1;
      //Brew_In = 1;   //test variable
      //Cold_Rinse = 1;
      //Clean_In = 1;

      if (WPS_In)
         BrewState = WPS_State;
      else if (Clean_In)
         BrewState = Clean_State;
      else if (Manual_In)
         BrewState = Manual_State;
      else if (Brew_In)
         BrewState = Safety_Check_State;     
   }   
   
   vTaskDelay(100 / portTICK_PERIOD_MS);
   Step_Active = 0;
   vTaskDelete(NULL);
}

void WPS (void)
{
   WPS_In = 0;             //Reset trigger variable

   strcpy (Stage,"WPS");
   strcpy (Step,"");
   printf("-----%s: %s-----\n", Stage, Step);

   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   Step_Active = 0;
   BrewState = Passive_State;
   vTaskDelete(NULL);
}

void Clean (void)
{
   Clean_In = 0;             //Reset trigger variable
   
   strcpy (Stage,"Cleaning");
   
   if (!Cold_Rinse)  //Full clean
   {
      printf("Ensure mash tun or cooler is connected to valves 2 & 3\n");
      strcpy (User_Int_Required,"Ensure mash tun or cooler is connected to valves 2 & 3");
      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1;  //Wait for user confirmation
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for .1 second
   }

   if (Auto_Fill)
   {
      strcpy (Step,"Auto-fill");
      printf("-----%s: %s-----\n", Stage, Step);

      if (External_Connection)
      {
         printf("Ensure hose is connected to extenal source and tap is turned on\n");
         strcpy (User_Int_Required,"Ensure hose is connected to extenal source and tap is turned on");
      }
      else
      {
         strcpy (User_Int_Required,"Ensure external tank is filled with cleaning water and connect hose to tank");
         printf("Ensure external tank is filled with cleaning water and connect hose to tank\n");
      }

      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1;  //Wait for user confirmation
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for .1 second
      
      Fill.Target_Volume = 5;  //Re-define target volume. Maybe more than 5L required?

      Stage_complete = 0;
      xTaskCreate(
         Auto_Run,                  //function name
         "Auto Control",            //function description
         2048,                      //stack size
         &Fill,                     //task parameters
         1,                         //task priority
         &Auto_Task                 //task handle
      );
      while (!Stage_complete)    //Wait for kettle to fill with target volume
      {
         vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
      }
   }
   else
   {
      printf("Ensure kettle tank is filled with cleaning water\n");
      strcpy (User_Int_Required,"Ensure kettle tank is filled with cleaning water");

      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1;  //Wait for user confirmation
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for .1 second
   }

   if (Cold_Rinse)
   {
      strcpy (Step,"Cold Rinse");      //1 min cold rinse through main loop
      printf("-----%s: %s-----\n", Stage, Step);
      
      Sanitize.Valve1 = 1;          //Re-define valve1
      Sanitize.Target_Time = 1;     //Re-define target time
   }

   else  //Full clean
   {
      strcpy (Step,"Full Clean");         //15 mins at 80 deg C, cycle through mash tun as well
      printf("-----%s: %s-----\n", Stage, Step);

      Sanitize.Valve1 = 1;          //Re-define valve1
      Sanitize.Target_Temp = 80;    //Hot enough to pasterize
      Sanitize.Target_Time = 15;     //Re-define target time
      Sanitize.Heater = 1;
   }

   Stage_complete = 0;
   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Sanitize,                     //task parameters
      1,                         //task priority
      &Auto_Task                 //task handle
   );
   while (!Stage_complete)    //Wait for target time
   {
      if ((Absolute_Seconds_Remaining == 600) && (Main_Config == 1)) //10 min remaining
      {
         valve_sparge_in.internal = 0; //Internal / external toggle
         valve_set_position(100, &valve_sparge_in); //Position defined by flow rate setting
         valve_sparge_out.internal = 0; //Internal / external toggle
         valve_set_position(100, &valve_sparge_out); //Position defined by flow rate setting
         vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
      }
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   printf("Ensure return hose is leading to a drain\n");
   strcpy (User_Int_Required,"Ensure return hose is leading to a drain");

   User_Int_Rqd = 1; //Flag that interaction is required
   Pause_In = 1; //Wait for user confirmation
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for .1 seconds
 
   PumpRelay(On);
  
   vTaskDelay(10000 / portTICK_PERIOD_MS); //pause task for 10 seconds test

   // while (FlowRate != 0) //wait for tranfer to complete
   // {
   //    vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 second
   // }

   PumpRelay(Off);
   Step_Active = 0;
   BrewState = Passive_State;
   vTaskDelete(NULL);
}

void Manual (void)
{
   Manual_In = 0;             //Reset trigger variable

   Man_EN = 1;    //Manual settings that are always able to be updated by user
   xTaskCreate(
      Manual_User_Input,         //function name
      "Manual_User_Input",       //function description
      2048,                      //stack size
      NULL,                      //task parameters
      1,                         //task priority
      NULL                       //task handle
   );
   
   strcpy (Stage,"Manual");
   
   ManState = Full_Man; // Test

   while (BrewState == Manual_State)
   {
      switch (ManState)
      {
         case Full_Man:
         {
            strcpy (Step,"Full Manual");
            printf("-----%s: %s-----\n", Stage, Step);
            
            while(ManState == Full_Man)
            {
               vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 second
            }
            break;
         }
              
         case Manual_Timer:
         {
            strcpy (Step,"Timer");
            printf("-----%s: %s-----\n", Stage, Step);

            if(Man_Temp != 0)
            {
               strcpy (Auto_Process,"Waiting until target temp");
               printf("*%s*\n", Auto_Process);

               while (!Temp_Reached) //Wait for temp to be reached
               {
                  vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
               }
            }

            strcpy (Auto_Process,"Waiting until target time");
            printf("*%s*\n", Auto_Process);

            while ((Man_Time*60) > Timer)    //conversion to seconds
            {
               TickType_t xLastWakeTime = xTaskGetTickCount(); //Saves LastWakeTime for use with vTaskDelayUntil

               Absolute_Seconds_Remaining = ((Man_Time*60)-Timer);
               Minutes_Remaining = (((Man_Time*60)-Timer)/60);
               Seconds_Remaining = (Absolute_Seconds_Remaining-(Minutes_Remaining*60));
               printf("Time Remaining: %d:%d\n", Minutes_Remaining, Seconds_Remaining);
               
               Count_Update = 1;       //Fix for hop appition call happening twice

               vTaskDelayUntil(&xLastWakeTime, 100); //pause task for exactly 1 second

               Timer ++;          //Increment timer   
            }

            Timer = 0;
            Man_Heater = 0;   //Turn heater off
            Man_Pump = 0;     //Turn pump off
            ManState = Full_Man;
            break;
         }

         case Manual_Volume:
         {
            strcpy (Step,"Volume Tranfer");
            printf("-----%s: %s-----\n", Stage, Step);

            while (Man_Volume == 0) //wait for user to input volume
            {
               vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 seconds
            }

            strcpy (Auto_Process,"Transfering target volume");
            printf("*%s*\n", Auto_Process);

            //Call volume function

            while (!Volume_Reached)
            {
                  vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
                  Volume_Reached=1;  //test
            }

            Man_Heater = 0;      //Turn heater off
            Man_Pump = 0;        //Turn pump off
            ManState = Full_Man;
            break;
         }
         
         case Manual_Instant_Heat:
         {
            strcpy (Step,"Instant Heat");
            printf("-----%s: %s-----\n", Stage, Step);

            while (Sparging.Instant_Temp == 0) //wait for user to input temp
            {
               vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 seconds
            }

            Stage_complete = 0;
            xTaskCreate(
               Instant_Heat,                //function name
               "Instant_Heat",      //function description
               2048,                      //stack size
               &Sparging,        //task parameters
               1,                         //task priority
               &Instant_Heat_Task         //task handle
            );
            while (!Stage_complete) //Wait for instant_heat to complete
            {
               vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 seconds
            }

            Man_Heater = 0;      //Turn heater off
            Man_Pump = 0;        //Turn pump off
            ManState = Full_Man;
            break;
         }

         default:
            ManState = Full_Man;
         break;
      }
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
   }


   Step_Active = 0;
   Man_EN = 0;
   BrewState = Passive_State;
   Manual_Task = NULL;
   vTaskDelete(NULL);
}


void Manual_User_Input (void) //continuosly update controls with user input
{
   //Manual_Config(); // apply user setting updates. Shift to inside while loop for final build

   while (Man_EN)
   {
      if(!Paused)
      {
         //Manual_Config(); // apply user setting updates

         Sparging.Instant_Temp = Man_Temp;
         Sparging.Instant_Volume = Man_Volume;

         //Check and set valve direction and flow rate

         if ((Man_Valve1 != Valve1_State) ||(Man_Valve1_Flow != Current_Flow1)) //if position or flow changed
         {
            valve_tap_in.internal = Man_Valve1; //Internal / external toggle
            if (ManState != Manual_Instant_Heat)
            {
               Current_Flow1 = Man_Valve1_Flow;
               valve_set_position(Current_Flow1, &valve_tap_in); //Position defined by flow rate setting
            }
         }
         if ((Man_Valve2 != Valve2_State)||(Man_Valve2_Flow != Current_Flow2)) //if position or flow changed
         {
            valve_sparge_in.internal = Man_Valve2; //Internal / external toggle
            if (ManState != Manual_Instant_Heat)
               Current_Flow2 = Man_Valve2_Flow;
            else
            {
               Current_Flow2 = 100;
               Man_Valve2_Flow = 100;
            }
            valve_set_position(Current_Flow2, &valve_sparge_in); //Position defined by flow rate setting
         }
         
         if ((Man_Valve3 != Valve3_State)||(Man_Valve3_Flow != Current_Flow3)) //if position or flow changed
         {
            valve_sparge_out.internal = Man_Valve3; //Internal / external toggle
            if (ManState != Manual_Instant_Heat)
               Current_Flow3 = Man_Valve3_Flow;
            else
            {
               Current_Flow3 = 100;
               Man_Valve3_Flow = 100;
            }
            valve_set_position(Current_Flow3, &valve_sparge_out); //Position defined by flow rate setting
         }

         Temp = Man_Temp;

         if (Man_Pump != Pump_Is_On)
            PumpRelay(Man_Pump); 

         if (ManState != Manual_Instant_Heat)
         {
            if (Man_Heater != Heater_Is_On)
               HeaterRelay(Man_Heater);
            
            Manual_Duty = Man_Heater_Power;
            Sensor = Man_Sensor;

            if (Temp != 0)
               Auto_PID = 1;  //Use heater PID
            else
               Auto_PID = 0;  //Use manual duty cycle

            if(!PWM_En)
            {
               PWM_En = 1;
               xTaskCreate(
                  Heater_PWM,                //function name
                  "Heater PWM Control",      //function description
                  2048,                      //stack size
                  NULL,                      //task parameters
                  1,                         //task priority
                  NULL                      //task handle
               );
            }
         }
      }

      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 second
   }
   
   vTaskDelete(NULL);
}


void Safety_Check (void)
{
   Brew_In = 0;             //Reset trigger variable
   
   strcpy (Stage,"Safety Check");
   strcpy (Step,"");
   printf("-----%s: %s-----\n", Stage, Step);

   printf("Grain Weight: %.2fKG\n", Grain_Weight);
   printf("Kettle Recipe Volume: %.1fL\n", Total_Brew_Volume);
  
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
         sprintf(User_Int_Required, "***WARNING*** Recipe exceeds kettle volume by: %.1fL", (Total_Brew_Volume-Kettle_Safe));
         printf("%s\n", User_Int_Required);
         User_Int_Rqd = 1; //Flag that interaction is required
         Pause_In = 1;
         vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
      if (!Mash_Check)
      {
         sprintf(User_Int_Required, "***WARNING*** Recipe exceeds mash volume by: %.1fL", Mash_Exceeded);
         printf("%s\n", User_Int_Required);
         User_Int_Rqd = 1; //Flag that interaction is required
         Pause_In = 1;
         vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
      BrewState = Passive_State;
   }

   Step_Active = 0;
   Safety_Task = NULL;
   vTaskDelete(NULL);
}




void Mash (void)
{ 
   strcpy (Stage,"Mash");

   //Fill call

   if (Auto_Fill)
   {
      strcpy (Step,"Auto-fill");
      printf("-----%s: %s-----\n", Stage, Step);
      if (External_Connection)
      { 
         printf("Ensure hose is connected to extenal source and tap is turned on\n");
         strcpy (User_Int_Required,"Ensure hose is connected to extenal source and tap is turned on");
      }
      else
      {
         printf("Ensure external tank is filled with brewing water and connect input hose to tank\n");
         strcpy (User_Int_Required,"Ensure external tank is filled with brewing water and connect input hose to tank");
      }

      User_Int_Rqd = 1; //Flag that interaction is required         
      Pause_In = 1;
      vTaskDelay(1000 / portTICK_PERIOD_MS);

      Stage_complete = 0;
      xTaskCreate(
         Auto_Run,                  //function name
         "Auto Control",            //function description
         2048,                      //stack size
         &Fill,                     //task parameters
         1,                         //task priority
         &Auto_Task                 //task handle
      );
      
      vTaskDelay(1000 / portTICK_PERIOD_MS); //wait for water to get to heater

      HeaterRelay(On); //will cut down on time being able to heat water as it enters the kettle
      Auto_PID = 0;
      PWM_En = 1;
      Manual_Duty = 100;
      xTaskCreate(Heater_PWM, "Heater PWM Control", 2048, NULL, 1, NULL);  //

      while (!Stage_complete)
      {
         vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
      }

   }
   else  //manual fill
   {
      printf("Ensure kettle is filled with the desired amount of strike water\n");
      strcpy (User_Int_Required,"Ensure kettle is filled with the desired amount of strike water");
      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1;
      vTaskDelay(1000 / portTICK_PERIOD_MS);
   }


   strcpy (Step,"Strike");
   printf("-----%s: %s-----\n", Stage, Step);

   Stage_complete = 0;
   xTaskCreate(
      Auto_Run,                  //function name
      "Auto Control",            //function description
      2048,                      //stack size
      &Strike_Heat,             //task parameters
      1,                         //task priority
      &Auto_Task                       //task handle
   );
   if ((Main_Config == 1)&&Heating_Method)   //Full config with RIMS, testing required to see how well this works
   {
      while (!Temp_Reached) //Wait for temp to be reached
      {
         vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 seconds
      }

   //Move valves to mash tun
      valve_sparge_in.internal = 0;
      valve_set_position(Current_Flow2, &valve_sparge_in);
      valve_sparge_out.internal = 0;
      valve_set_position(Current_Flow3, &valve_sparge_out);
   }
   while (!Stage_complete)
   {
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   strcpy (Step,"Step 1");
   printf("-----%s: %s-----\n", Stage, Step);

   if (Main_Config != 1)   //BIAB
   {
      printf("Please insert BIAB into kettle and fill with grain\n");
      strcpy (User_Int_Required,"Please insert BIAB into kettle and fill with grain");
      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1; //****Wait for user to confirm bag is in place
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
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
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   strcpy (Step,"Step 2");
   printf("-----%s: %s-----\n", Stage, Step);

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
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   strcpy (Step,"Step 3");
   printf("-----%s: %s-----\n", Stage, Step);

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
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   strcpy (Step,"Step 4");
   printf("-----%s: %s-----\n", Stage, Step);

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
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   strcpy (Step,"Step 5");
   printf("-----%s: %s-----\n", Stage, Step);

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
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 1 second
   }

   HeaterRelay(Off);
   PumpRelay(Off);

   vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 seconds
   Step_Active = 0;
   BrewState = Sparge_State;
   Mash_Task = NULL;
   vTaskDelete(NULL);
}



void Sparge (void)
{
   strcpy (Stage,"Sparge");
   strcpy (Step,"");
   printf("-----%s: %s-----\n", Stage, Step);

   if ((Sparge_Water_Volume != 0)&&(Main_Config != 3)) //Sparge required
   {
      valve_tap_in.internal = 0; //Set to external

      if (!Heating_Method) //Internal boiler
      {
         printf("Please confirm external vessel is filled with pre-heated sparge water\n");
         strcpy (User_Int_Required,"Please confirm external vessel is filled with pre-heated sparge water");
         
         if (Main_Config == 2)
         {
            printf("Please confirm external vessel is filled with pre-heated sparge water. Position BIAB and return hose securely above kettle\n");
            strcpy (User_Int_Required,"Please confirm external vessel is filled with pre-heated sparge water. Position BIAB and return hose securely above kettle");
         }

         User_Int_Rqd = 1; //Flag that interaction is required
         Pause_In = 1;  //wait for user confirmation
         vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for .1 seconds

         PumpRelay(On);
         //volume transfer function of pre-heated sparge water
      }

      else  //Instant heat sparge 
      {  
         if ((Main_Config == 2)&&(External_Connection))
         {
            printf("Please position BIAB and return hose securely above kettle. Confirm external water source is connected and turned on\n");
            strcpy (User_Int_Required,"Please position BIAB and return hose securely above kettle. Confirm external water source is connected and turned on");
         }
         else if ((Main_Config == 2)&&(!External_Connection))
         {
            printf("Please position BIAB and return hose securely above kettle. Confirm external vessel is filled with sparge water\n");
            strcpy (User_Int_Required,"Please position BIAB and return hose securely above kettle. Confirm external vessel is filled with sparge water");
         }
         
         else if (External_Connection)
         {
            printf("Please confirm external water source is connected and turned on\n");
            strcpy (User_Int_Required,"Please confirm external water source is connected and turned on");
         }
         else
         {
            printf("Please confirm external vessel is filled with sparge water\n");
            strcpy (User_Int_Required,"Please confirm external vessel is filled with sparge water");
         }

         User_Int_Rqd = 1; //Flag that interaction is required
         Pause_In = 1;
         vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 seconds

         valve_set_position(20, &valve_tap_in); //Set valve 1 external at 20% flowrate. Instant heat will then overide setting

         Stage_complete = 0;
         xTaskCreate(
            Instant_Heat,                //function name
            "Instant_Heat",      //function description
            2048,                      //stack size
            &Sparging,        //task parameters
            1,                         //task priority
            &Instant_Heat_Task         //task handle
         );

         while (!Stage_complete) //Wait for sparge to complete
         {
            vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 seconds
         }
         
      }
      if (Main_Config == 2)
      {
         printf("Please place return hose securely in kettle\n");
         strcpy (User_Int_Required,"Please place return hose securely in kettle");
         User_Int_Rqd = 1; //Flag that interaction is required
         Pause_In = 1;
         vTaskDelay(1000 / portTICK_PERIOD_MS); //Wait for user input
      }
   
      if (Main_Config == 1)  //SpargeDrain
      {
         strcpy (Step,"Draining Mash");
         printf("-----%s: %s-----\n", Stage, Step);
         // while (Flowrate > xL) //Wait for x time or wait till flow < x value
         // {
         //    vTaskDelay(1000 / portTICK_PERIOD_MS);
         // }       
      }
   }
   HeaterRelay(Off);
   PumpRelay(Off);
   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   Step_Active = 0;
   BrewState = Boil_State;
   Sparge_Task = NULL;
   vTaskDelete(NULL);
}

void Boil (void)
{
   strcpy (Stage,"Boil");
   strcpy (Step,"");
   printf("-----%s: %s-----\n", Stage, Step);

   if (Main_Config == 3)   //BIAB
   {
      printf("Please remove BIAB from kettle\n");
      strcpy (User_Int_Required,"Please remove BIAB from kettle");
      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1;  //wait for user confirmation
      vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   }
   Stage_complete = 0;
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
         {
            strcpy (User_Adjunct_Required, "Please add: ");
            strcat(User_Adjunct_Required, Adjunct_Name_1);
            printf("%s\n",User_Adjunct_Required);
            User_Adjunct_Rqd = 1;
         }
         if ((Absolute_Seconds_Remaining == Adjunct_Time_2*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_2 == 0)&&(strcmp (Adjunct_Name_2,""))))
         {   
            strcpy (User_Adjunct_Required, "Please add: ");
            strcat (User_Adjunct_Required, Adjunct_Name_2);
            printf("%s\n",User_Adjunct_Required);
            User_Adjunct_Rqd = 1;
         }
         if ((Absolute_Seconds_Remaining == Adjunct_Time_3*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_3 == 0)&&(strcmp (Adjunct_Name_3,""))))
         {
            strcpy (User_Adjunct_Required, "Please add: ");
            strcat (User_Adjunct_Required, Adjunct_Name_3);
            printf("%s\n",User_Adjunct_Required);
            User_Adjunct_Rqd = 1;
         }
         if ((Absolute_Seconds_Remaining == Adjunct_Time_4*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_4 == 0)&&(strcmp (Adjunct_Name_4,""))))
         {
            strcpy (User_Adjunct_Required, "Please add: ");
            strcat (User_Adjunct_Required, Adjunct_Name_4);
            printf("%s\n",User_Adjunct_Required);
            User_Adjunct_Rqd = 1;
         }
         if ((Absolute_Seconds_Remaining == Adjunct_Time_5*60)||((Absolute_Seconds_Remaining == 1)&&(Adjunct_Time_5 == 0)&&(strcmp (Adjunct_Name_5,""))))
         {
            strcpy (User_Adjunct_Required, "Please add: ");
            strcat (User_Adjunct_Required, Adjunct_Name_5);
            printf("%s\n",User_Adjunct_Required);
            User_Adjunct_Rqd = 1;
         }
         if ((Absolute_Seconds_Remaining == 300)&& Cooling_Rqd) //5 minutes remaining, Prompts user to sterilize cooler
         {
            strcpy (User_Adjunct_Required,"Sterilize cooler in boiling wort if desired");
            printf("%s\n",User_Adjunct_Required);
            User_Adjunct_Rqd = 1;
         }

         Count_Update=0;    
      }

      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for .1 seconds
   }

   HeaterRelay(Off);
   PumpRelay(Off);

   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   Step_Active = 0;
   BrewState = Cooling_State;
   Boil_Task = NULL;
   vTaskDelete(NULL);
}

void Cooling (void)
{
   strcpy (Stage,"Cooling");
   strcpy (Step,"");
   printf("-----%s: %s-----\n", Stage, Step);

   if (Cooling_Rqd)
   {
      if (Main_Config == 1)
      {
         printf("Please disconnect mash tun and connect cooler\n");
         strcpy (User_Int_Required,"Please disconnect mash tun and connect cooler");
      }
      else if (Cooling_Method)   //BIAB and Pumped wort
      {
         printf("Please connect cooler to kettle return line\n");
         strcpy (User_Int_Required,"Please connect cooler to kettle return line");
      }
      else    ///BIAB pumped water
      {
         printf("Please connect cooler to external source input\n");
         strcpy (User_Int_Required,"Please connect cooler to external source input");
      }

      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1;  //Wait for user confirmation
      vTaskDelay(1000 / portTICK_PERIOD_MS); //executes pause before task create

      if (!Cooling_Method)    //Pumped water
      {
         printf("Please immerse cooler in kettle, ensure kettle return line leads to a drain\n");
         strcpy (User_Int_Required,"Please immerse cooler in kettle, ensure kettle return line leads to a drain");
      }

      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1;  //Wait for user confirmation
      vTaskDelay(1000 / portTICK_PERIOD_MS); //executes pause before task create
      

      Stage_complete = 0;
      xTaskCreate(
         Auto_Run,                  //function name
         "Auto Control",            //function description
         2048,                      //stack size
         &Cool,                  //task parameters
         1,                         //task priority
         &Auto_Task                  //task handle
      );
      
      while (!Stage_complete)
      {   
         vTaskDelay(100 / portTICK_PERIOD_MS); //.1 sec 
      }
      
      while (Cool.Target_Sensor != Cooling_Temp)   //Wait for target temp
      { 
         vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
         Cool.Target_Sensor = Cooling_Temp;  //Testbench setting
      }
      
   }
   //Check for pumped Water and transfer, if true carry out sanitize function
   if (((Cooling_Rqd)&&!Cooling_Method)&&Transfer_Method)
   {
      strcpy (Step,"Sanitizing lines");
      printf("-----%s: %s-----\n", Stage, Step);

      if (Main_Config !=1) 
      {
         printf("Please remove cooler\n");
         strcpy (User_Int_Required,"Please remove cooler");

         User_Int_Rqd = 1; //Flag that interaction is required
         Pause_In = 1;  // Wait for user input
         vTaskDelay(1000 / portTICK_PERIOD_MS); //execute pause
      }
      printf("Fill external vessel with 5L of sanitizer fluid. Place external source and kettle retun hoses into the sanitiser fluid\n");
      strcpy (User_Int_Required,"Fill external vessel with 5L of sanitizer fluid. Place external source and kettle retun hoses into the sanitiser fluid");

      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1;  // Wait for user input
      vTaskDelay(1000 / portTICK_PERIOD_MS); //execute pause
      
      Stage_complete = 0;
      xTaskCreate(
         Auto_Run,                  //function name
         "Auto Control",            //function description
         2048,                      //stack size
         &Sanitize,                  //task parameters
         1,                         //task priority
         &Auto_Task                  //task handle
      );

      while (!Stage_complete)
      {   
         vTaskDelay(100 / portTICK_PERIOD_MS); //1 sec 
      }
   }


   Step_Active = 0;
   BrewState = Transfer_State;
   Cooling_Task = NULL;
   vTaskDelete(NULL);
}

void Transfer (void)
{
   strcpy (Stage,"Transfer");
   strcpy (Step,"");
   printf("-----%s: %s-----\n", Stage, Step);

   if (Transfer_Method)
   {
      printf("Please configure kettle return hose for transfer\n");
      strcpy (User_Int_Required,"Please configure kettle return hose for transfer");
      
      User_Int_Rqd = 1; //Flag that interaction is required
      Pause_In = 1;
      vTaskDelay(1000 / portTICK_PERIOD_MS); //execute pause
      
      Stage_complete = 0;
      xTaskCreate(
         Auto_Run,                  //function name
         "Auto Control",            //function description
         2048,                      //stack size
         &Zeroise,                  //task parameters
         1,                         //task priority
         &Auto_Task                  //task handle
      );
      while (!Stage_complete)
      {   
         vTaskDelay(100 / portTICK_PERIOD_MS); //1 sec 
      }

      PumpRelay(On);
      //Flow volume measure on
      //while (Flow !=0)
      {
         //delay
      }

      vTaskDelay(5000 / portTICK_PERIOD_MS); //5 sec

      PumpRelay(Off);
   }


   vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
   Step_Active = 0;
   BrewState = Passive_State;      
   Transfer_Task = NULL;
   vTaskDelete(NULL);
}

void Pause(void)
{
   Pump_State = Pump_Is_On;   //Save relay states
   Heater_State = Heater_Is_On;
   Paused = 1;
   HeaterRelay(Off);
   PumpRelay(Off);
   
   //suspend running tasks
   if (Auto_Task != NULL)
      vTaskSuspend(Auto_Task);
   if (Instant_Heat_Task != NULL)
      vTaskSuspend(Instant_Heat_Task);
   if (WPS_Task != NULL)
      vTaskSuspend(WPS_Task);
   if (Cleaning_Task != NULL)
      vTaskSuspend(Cleaning_Task);
   if (Manual_Task != NULL)
      vTaskSuspend(Manual_Task);
   if (Safety_Task != NULL)
      vTaskSuspend(Safety_Task);
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

   while(!Pause_Button && Pause_In)  //Manual reset of hardcoded pause
   {
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task for 5 seconds, will be replaced by user unpause 
      Pause_Button = gpio_get_level (TEST_PAUSE_PIN);
   }
   Pause_In = 0;
   while(Pause_Button)  //Manual Pause and manual reset
   {
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task
      Pause_Button = gpio_get_level (TEST_PAUSE_PIN);
   }

   //unpause
   
   Paused = 0;
   printf("Resumed\n");

   PumpRelay(Pump_State);     //Reapply relay states
   HeaterRelay(Heater_State);

   strcpy (User_Int_Required,"");
   User_Int_Rqd = 0;
   
   //resume running tasks
   if (WPS_Task != NULL)
      vTaskResume(WPS_Task);
   if (Cleaning_Task != NULL)
      vTaskResume(Cleaning_Task);
   if (Manual_Task != NULL)
      vTaskResume(Manual_Task);
   if (Safety_Task != NULL)
      vTaskResume(Safety_Task);
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
   if (Instant_Heat_Task != NULL)
      vTaskResume(Instant_Heat_Task);
   if (Auto_Task != NULL)
      vTaskResume(Auto_Task);

   Pause_Task = NULL;
   vTaskDelete(NULL);
}

void Cancel(void)
{
   printf("Canceled\n");

   while (Pause_Button)        //Only required for testing. Prevents SS from physical button
   {
      vTaskDelay(100 / portTICK_PERIOD_MS); //pause task
   }
   

   Step_Active = 0;
   Man_EN = 0;

   if (WPS_Task != NULL)
      vTaskDelete(WPS_Task);
   if (Cleaning_Task != NULL)
      vTaskDelete(Cleaning_Task);
   if (Manual_Task != NULL)
      vTaskDelete(Manual_Task);
   if (Safety_Task != NULL)
      vTaskDelete(Safety_Task);
   if (Mash_Task != NULL)
      vTaskDelete(Mash_Task);
   if (Sparge_Task != NULL)
      vTaskDelete(Sparge_Task);
   if (Boil_Task != NULL)
      vTaskDelete(Boil_Task);
   if (Cooling_Task != NULL)
      vTaskDelete(Cooling_Task);
   if (Transfer_Task != NULL)
      vTaskDelete(Transfer_Task);
   if (Auto_Task != NULL)
      vTaskDelete(Auto_Task);
   if (Pause_Task != NULL)
      vTaskDelete(Pause_Task);

   Pause_Task = NULL;      //Fixes issue with multiple cancels causing reboot
   Auto_Task = NULL;
   Transfer_Task = NULL;
   Cooling_Task = NULL;
   Boil_Task = NULL;
   Sparge_Task = NULL;
   Mash_Task = NULL;
   Manual_Task = NULL;
   Cleaning_Task = NULL;
   WPS_Task = NULL;

   BrewState = Passive_State;
   vTaskDelete(NULL);
}