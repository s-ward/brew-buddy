#include <stdio.h>
#include "HeaterPID.h"

int Heater_Duty_Cycle;

int Heater_PID (int Target_Temp, int Target_Sensor)
{

    Heater_Duty_Cycle = 40;         //manual value atm, proper PID function will update this automatically
    //printf("%d\n", Heater_Duty_Cycle);

    //if ((Target_Temp-1) <= Actual_Temp <= (Target_Temp+1))
        Temp_Reached = 1;

    //else
        //Temp_Reached = 0;

    return (Heater_Duty_Cycle);
}