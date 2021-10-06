#include <stdio.h>
#include "ActiveRecipe.h"
#include "Auto_Run.h"
#include "Auto_Run_Setup.h"
#include "Instant_Heat.h"
#include "string.h"

void ActiveRecipe (int grainweight, int mashwatervol, int striketemp, int flowrate,
    int mashtime1, int mashtime2, int mashtime3, int mashtime4, int mashtime5,
    int mashtemp1, int mashtemp2, int mashtemp3, int mashtemp4, int mashtemp5,
    int spargewatervol, int spargetemp, int boiltime,
    char adjunctname1, char adjunctname2, char adjunctname3, char adjunctname4, char adjunctname5,
    int adjuncttime1, int adjuncttime2, int adjuncttime3, int adjuncttime4, int adjuncttime5,
    int cooltemp)
{
    //Loads active recipe data into global variables
    //Manual entry at the moment
    //will eventually refrence register where brew is stored

    //Control Variables, required for recipe
    
    //*********MASH*********

    //KGs
    Grain_Weight = grainweight;

    //Liters
    Mash_Water_Volume = mashwatervol;

    //Deg C
    Strike_Temp = striketemp;

    //0-100% of valve open
    Mash_Flow_Rate = flowrate;

    //minutes
    Mash_Time_1 = mashtime1;
    Mash_Time_2 = mashtime2;
    Mash_Time_3 = mashtime3;
    Mash_Time_4 = mashtime4;
    Mash_Time_5 = mashtime5;

    //Deg C
    Mash_Temp_1 = mashtemp1;
    Mash_Temp_2 = mashtemp2;
    Mash_Temp_3 = mashtemp3;
    Mash_Temp_4 = mashtemp4;
    Mash_Temp_5 = mashtemp5;

    //*********SPARGE*********

    //Liters
    Sparge_Water_Volume = spargewatervol; 

    //Deg C
    Sparge_Temp = spargetemp;
 
    //*********BOIL*********

    //Minutes
    Boil_Time = boiltime;

    //Adjunct Names
    strcpy (Adjunct_Name_1,adjunctname1);
    strcpy (Adjunct_Name_2,adjunctname2);
    strcpy (Adjunct_Name_3,adjunctname3);
    strcpy (Adjunct_Name_4,adjunctname4);
    strcpy (Adjunct_Name_5,adjunctname5);

    //minutes
    Adjunct_Time_1 = adjuncttime1;    //@x minutes remaining in boil
    Adjunct_Time_2 = adjuncttime2;
    Adjunct_Time_3 = adjuncttime3;
    Adjunct_Time_4 = adjuncttime4;
    Adjunct_Time_5 = adjuncttime5;


    //*********COOLING*********

    //Deg C
    Cooling_Temp = 25;

    //*********Calculation Variables*********
    Total_Brew_Volume = (Mash_Water_Volume + Sparge_Water_Volume - Grain_Weight);

    Auto_Run_Setup(); //Assign equip config and recipe data to auto_run struct
}