#include <stdio.h>
#include "Auto_Run_Setup.h"
#include "ActiveRecipe.h"
#include "EquipConfig.h"
#include "Auto_Run.h"



void Auto_Run_Setup (void)
{

    //*********Strike Water*********
        Strike_Heat.Valve1 = 0; //from kettle
        Strike_Heat.Valve2 = 0; //to valve 3
        Strike_Heat.Valve3 = 0; //from valve 2
        Strike_Heat.FlowCtrl = Mash_Flow_Rate;
        Strike_Heat.Target_Temp = Strike_Temp;
        Strike_Heat.Target_Sensor = 2;  //Middle sensor
        Strike_Heat.Pump = 1;
        Strike_Heat.Heater = 1;
        Strike_Heat.Target_Volume = Mash_Water_Volume;
        Strike_Heat.Target_Time = 0;
        
    //*********Mash Step 1*********
        Mash1.Valve1 = 0;   //from kettle
        Mash1.Valve2 = 1;   //to mash tun
        Mash1.Valve3 = 1;   //from mash tun
        Mash1.FlowCtrl = Mash_Flow_Rate;
        Mash1.Target_Temp = Mash_Temp_1;
        Mash1.Target_Sensor = 2;
        Mash1.Pump = 1;
        Mash1.Heater = 1;
        Mash1.Target_Volume = 0;
        Mash1.Target_Time = Mash_Time_1;
        
    //*********Mash Step 2*********
        Mash2.Valve1 = 0;   //from kettle
        Mash2.Valve2 = 1;   //to mash tun
        Mash2.Valve3 = 1;   //from mash tun
        Mash2.FlowCtrl = Mash_Flow_Rate;
        Mash2.Target_Temp = Mash_Temp_2;
        Mash2.Target_Sensor = 2;
        Mash2.Pump = 1;
        Mash2.Heater = 1;
        Mash2.Target_Volume = 0;
        Mash2.Target_Time = Mash_Time_2;

    //*********Mash Step 3*********
        Mash3.Valve1 = 0;   //from kettle
        Mash3.Valve2 = 1;   //to mash tun
        Mash3.Valve3 = 1;   //from mash tun
        Mash3.FlowCtrl = Mash_Flow_Rate;
        Mash3.Target_Temp = Mash_Temp_3;
        Mash3.Target_Sensor = 2;
        Mash3.Pump = 1;
        Mash3.Heater = 1;
        Mash3.Target_Volume = 0;
        Mash3.Target_Time = Mash_Time_3;

    //*********Mash Step 4*********
        Mash4.Valve1 = 0;   //from kettle
        Mash4.Valve2 = 1;   //to mash tun
        Mash4.Valve3 = 1;   //from mash tun
        Mash4.FlowCtrl = Mash_Flow_Rate;
        Mash4.Target_Temp = Mash_Temp_4;
        Mash4.Target_Sensor = 2;
        Mash4.Pump = 1;
        Mash4.Heater = 1;
        Mash4.Target_Volume = 0;
        Mash4.Target_Time = Mash_Time_4;

    //*********Mash Step 5*********
        Mash4.Valve1 = 0;   //from kettle
        Mash4.Valve2 = 1;   //to mash tun
        Mash4.Valve3 = 1;   //from mash tun
        Mash4.FlowCtrl = Mash_Flow_Rate;
        Mash4.Target_Temp = Mash_Temp_5;
        Mash4.Target_Sensor = 2;
        Mash4.Pump = 1;
        Mash4.Heater = 1;
        Mash4.Target_Volume = 0;
        Mash4.Target_Time = Mash_Time_5;


//*********Boil*********
        Boiling.Valve1 = 0;   //from kettle
        Boiling.Valve2 = 0;   //to valve 3
        Boiling.Valve3 = 0;   //to valve 3
        Boiling.FlowCtrl = 100;            //Max? may be better to set to Mash_Flow_Rate but testing required
        Boiling.Target_Temp = 100;         //Maybe 99 to prevent steam issues though. testing required
        Boiling.Target_Sensor = 2;
        Boiling.Pump = 1;
        Boiling.Heater = 1;
        Boiling.Target_Volume = 0;
        Boiling.Target_Time = Boil_Time;

        if (Main_Config !=1)    //BIAB differences
        {
            Strike_Heat.Target_Volume = 0;
            Strike_Heat.Target_Sensor = 1;  //Bottom sensor

            Mash1.Valve2 = 0; //to valve 3
            Mash1.Valve3 = 0; //from valve 2

            Mash2.Valve2 = 0; //to valve 3
            Mash2.Valve3 = 0; //from valve 2

            Mash3.Valve2 = 0; //to valve 3
            Mash3.Valve3 = 0; //from valve 2

            Mash4.Valve2 = 0; //to valve 3
            Mash4.Valve3 = 0; //from valve 2

            Mash5.Valve2 = 0; //to valve 3
            Mash5.Valve3 = 0; //from valve 2
        }
}