#include <stdio.h>
#include "ActiveRecipe.h"
#include "Auto_Run.h"
#include "Auto_Run_Setup.h"
#include "Instant_Heat.h"
#include "string.h"

void ActiveRecipe (void)
{
    //Loads active recipe data into global variables
    //Manual entry at the moment
    //will eventually refrence register where brew is stored

    //Control Variables, required for recipe
    
    //*********MASH*********

    //KGs
    Grain_Weight = 5.6;

    //Liters
    Mash_Water_Volume = 15;

    //Deg C
    Strike_Temp = 75;

    //0-100% of valve open
    Mash_Flow_Rate = 9;

    //minutes
    Mash_Time_1 = 0;
    Mash_Time_2 = 0;
    Mash_Time_3 = 0;
    Mash_Time_4 = 0;
    Mash_Time_5 = 0;

    //Deg C
    Mash_Temp_1 = 65;
    Mash_Temp_2 = 67;
    Mash_Temp_3 = 75;
    Mash_Temp_4 = 0;
    Mash_Temp_5 = 0;

    //*********SPARGE*********

    //Liters
    Sparge_Water_Volume = 20; 

    //Deg C
    Sparge_Temp = 75;
 
    //*********BOIL*********

    //Minutes
    Boil_Time = 1;

    //Adjunct Names
    strcpy (Adjunct_Name_1,"Mosaic 20g");
    strcpy (Adjunct_Name_2,"Fuggle 40g");
    strcpy (Adjunct_Name_3,"Hallertau 10g");
    strcpy (Adjunct_Name_4,"Irish Moss 5g");
    strcpy (Adjunct_Name_5,"Kent Golding 30g");

    //minutes
    Adjunct_Time_1 = 1;    //@x minutes remaining in boil
    Adjunct_Time_2 = 2;
    Adjunct_Time_3 = 3;
    Adjunct_Time_4 = 4;
    Adjunct_Time_5 = 0;


    //*********COOLING*********

    //Deg C
    Cooling_Temp = 25;

    //*********Calculation Variables*********
    Total_Brew_Volume = (Mash_Water_Volume + Sparge_Water_Volume - Grain_Weight);

    Auto_Run_Setup(); //Assign equip config and recipe data to auto_run struct
}