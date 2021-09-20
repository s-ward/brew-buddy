#include <stdio.h>
#include "HeaterRelay.h"
#include "driver/gpio.h"
#include "PumpRelay.h"

#define Heater_Relay_Pin 27
#define Heater_On (gpio_set_level(Heater_Relay_Pin, 1))
#define Heater_Off (gpio_set_level(Heater_Relay_Pin, 0))

void HeaterRelay (int Heater_Ctrl)
{
    gpio_set_direction(Heater_Relay_Pin, GPIO_MODE_OUTPUT);

    if (!Heater_Ctrl)
    {
        Heater_Off;
        Heater_Is_On = 0;
    }
    else if (Pump_Is_On || External_Safe) //Safety check, if pump is on or externally connected water flowing
    {
        Heater_On;
        Heater_Is_On = 1;
    }
}