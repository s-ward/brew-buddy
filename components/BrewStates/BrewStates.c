#include <stdio.h>
#include "BrewStates.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum BrewStates {Passive, Test, WPS, Clean, Manual, Safety_Check, Mash, Sparge, Boil, Cooling, Transfer};
char BrewState /*= Passive*/;

void Brew_States (void)
{
     while(1)
   {
      switch (BrewState)
      {
         case Passive:
            printf("Passive\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
            BrewState = Test;
           // Passive();
            break;

      //    case WPS:
      //       WPS();
      //       break;

      //    case Clean:
      //       Clean();
      //       break;

      //    case Manual:
      //       Manual();
      //       break;

      //    case Safety_Check:
      //       Safety_Check();
      //       break;

      //    case Mash:
      //       Mash();
      //       break;

      //    case Sparge:
      //       Sparge();
      //       break;

      //    case Boil:
      //       Boil();
      //       break;

      //    case Cooling:
      //       Cooling();
      //       break;

      //    case Transfer:
      //       Transfer();
      //       break;

         case Test:
            printf("Default\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS); //pause task for 1 second
            BrewState = Passive;
           // Passive();
            break;
      }
     
   }
}