#include <stdio.h>
#include "HeaterPID.h"

int Heater_PID (void)
{
    Heater_Duty_Cycle = 10;         //manual value atm, proper PID function will update this automatically
    return (Heater_Duty_Cycle);
}