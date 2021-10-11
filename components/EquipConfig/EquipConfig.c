#include <stdio.h>
#include "EquipConfig.h"
#include "string.h"

void EquipConfig (int main_config, int kettlevolume, int mashtunvolume, int external_connection, int pumpedtransfer,
                    char *units, int leadtime, char *heatingmethod, char *coolingmethod, int auto_fill, int safety_margin)
{
    //Loads config data into global variables
    //Manual entry at the moment
    //will eventually refrence register where default config is stored

    Main_Config = main_config;    //1 = full, 2 = BIAB + Sparge, 3 = BIAB
    Kettle_Volume = kettlevolume; //0-127
    Mash_Tun_Volume = mashtunvolume; //0-127
    External_Connection = external_connection; //0 = tank, 1 = tap

    printf("Kettle volume assigned: %d\n",Kettle_Volume);

    if (!strcmp (coolingmethod,"None"))
        Cooling_Rqd = 0;  // 0 = n, 1 = y

    else if (!strcmp (coolingmethod,"Wort Recirc"))
    {
        Cooling_Rqd = 1;  // 0 = n, 1 = y
        Cooling_Method = 1; // 0 = pumped water, 1 = pumped wort
    }
    else if (!strcmp (coolingmethod,"Water Recirc"))
    {
        Cooling_Rqd = 1;  // 0 = n, 1 = y
        Cooling_Method = 0; // 0 = pumped water, 1 = pumped wort
    }

    Transfer_Method = pumpedtransfer; //0 = manual, 1 = pumped

    if (!strcmp (heatingmethod,"RIMS"))
        Heating_Method = 1; //0 = boiler, 1 = RIMS
    else
        Heating_Method = 0; //0 = boiler, 1 = RIMS

    Auto_Fill = auto_fill;  //0 = manual, 1 = autofill kettle for mash
    
    Safety_Margin = safety_margin; //xL of safety margin for kettle and mash tun (Hardcoded)
}

void Manual_Config(int valve1, int valve2, int valve3, int targetflow1, int targetflow2, int targetflow3, int pump, 
    int heater, int heater_power, int temp_sensor, int target_temp, int time, int volume)
{
    Man_Valve1 = valve1;     //Valve 1 position 1 internal, 0 external
    Man_Valve2 = valve2;
    Man_Valve3 = valve3;

    Man_Valve1_Flow = targetflow1;   //Valve 1 flow control 0 - 100%
    Man_Valve2_Flow = targetflow2;
    Man_Valve3_Flow = targetflow3;

    Man_Pump = pump;       //1 0n 0 off

    Man_Heater = heater;         //1 on 0 off
    Man_Heater_Power = heater_power;  // 0 - 100%

    Man_Sensor = temp_sensor;     //Temp sensor select 1-3
    Man_Temp = target_temp;       //Target temp 0 -100

    Man_Time = time;       //Target time minutes

    Man_Volume = volume;     //Target volume liters
}