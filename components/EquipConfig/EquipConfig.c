#include <stdio.h>
#include "EquipConfig.h"

void EquipConfig (void)
{
    //Loads config data into global variables
    //Manual entry at the moment
    //will eventually refrence register where default config is stored

    Main_Config = 1;    //1 = full, 2 = BIAB + Sparge, 3 = BIAB
    Kettle_Volume = 37; //0-127
    Mash_Tun_Volume = 60; //0-127
    External_Connection = 0; //0 = tank, 1 = tap
    Cooling_Rqd = 1;  // 0 = n, 1 = y
    Cooling_Method = 0; // 0 = pumped water, 1 = pumped wort
    Transfer_Method = 1; //0 = manual, 1 = pumped
    Heating_Method = 1; //0 = boiler, 1 = RIMS

    Auto_Fill = 1;  //0 = manual, 1 = autofill kettle for mash
    
    Safety_Margin = 5; //xL of safety margin for kettle and mash tun (Hardcoded)
}

void Manual_Config(void)
{
    Man_Valve1 = 1;     //Valve 1 position 1 internal, 0 external
    Man_Valve2 = 1;
    Man_Valve3 = 1;

    Man_Valve1_Flow = 10;   //Valve 1 flow control 0 - 100%
    Man_Valve2_Flow = 70;
    Man_Valve3_Flow = 30;

    Man_Pump = 0;       //1 0n 0 off

    Man_Heater = 1;         //1 on 0 off
    Man_Heater_Power = 70;  // 0 - 100%

    Man_Sensor = 1;     //Temp sensor select 1-3
    Man_Temp = 2;       //Target temp 0 -100

    Man_Time = 1;       //Target time minutes

    Man_Volume = 1;     //Target volume liters
}