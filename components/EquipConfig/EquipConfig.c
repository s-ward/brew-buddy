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
    Cooling_Rqd = 0;  // 0 = n, 1 = y
    Cooling_Method = 1; // 0 = pumped water, 1 = pumped wort
    Transfer_Method = 1; //0 = manual, 1 = pumped
    Heating_Method = 0; //0 = boiler, 1 = RIMS

    Auto_Fill = 0;  //0 = manual, 1 = autofill kettle for mash
    
    Safety_Margin = 5; //xL of safety margin for kettle and mash tun (Hardcoded)
}