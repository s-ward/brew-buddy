#include <stdio.h>
#include "PumpRelay.h"
#include "HeaterRelay.h"
#include "driver/gpio.h"

#define Pump_Relay_Pin 26
#define Pump_On (gpio_set_level(Pump_Relay_Pin, 1))
#define Pump_Off (gpio_set_level(Pump_Relay_Pin, 0))

void PumpRelay (int Pump_Ctrl)
{
    gpio_set_direction(Pump_Relay_Pin, GPIO_MODE_OUTPUT);

    if (!Pump_Ctrl)
    {
        Pump_Off;
        Pump_Is_On = 0;
        HeaterRelay(Pump_Ctrl);
    }
    else
    {
        Pump_On;
        Pump_Is_On = 1;
    }
}