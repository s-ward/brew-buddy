#include <stdio.h>
#include "HeaterPID.h"

int Heater_Duty_Cycle;

int Heater_PID (void)
{
    Heater_Duty_Cycle = 0;         //manual value atm, proper PID function will update this automatically
    //printf("%d\n", Heater_Duty_Cycle);
    return (Heater_Duty_Cycle);
}