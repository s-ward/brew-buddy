#include <stdio.h>
#include "HeaterPID.h"

int Heater_PID (int Heater_Duty_Cycle)
{
    if (Heater_Duty_Cycle ==0)      //If manual heater power setting == 0, use PID, else return manual setting
    {
        //PID code here
        Heater_Duty_Cycle = 2;         //manual value atm, proper PID function will update this automatically
    }
    return (Heater_Duty_Cycle);
}