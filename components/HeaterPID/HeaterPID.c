#include <stdio.h>
#include "HeaterPID.h"

int Heater_Duty_Cycle; //0-100 PWM value, global variable that is changed through Heater_PID function

int Heater_PID (void)
{
    Heater_Duty_Cycle = 99;         //manual value atm, proper PID function will update this automatically
    return (Heater_Duty_Cycle);
}