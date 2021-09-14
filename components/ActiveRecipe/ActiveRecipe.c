#include <stdio.h>
#include "ActiveRecipe.h"
#include "string.h"

void ActiveRecipe (void)
{
    //Loads active recipe data into global variables
    //Manual entry at the moment
    //will eventually refrence register where default config is stored
    

    //Passive Variables
    //These may be required to pass to the webserver? otherwise can be removed 

    //Recipe Name
    strcpy (Recipe_Name, "Test recipe");

    //Control Variables, required for recipe
    
    //*********MASH*********

    //KGs
    Grain_Weight = 5.6;

    //Liters
    Mash_Water_Volume = 30;

    //Deg C
    Strike_Temp = 75;

    //0-100% of valve open
    Mash_Flow_Rate = 50;

    //minutes
    Mash_Time_1 = 20;
    Mash_Time_2 = 40;
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
    Sparge_Water_Volume = 30; 

    //Deg C
    Sparge_Temp = 75;
 
    //*********BOIL*********

    //Minutes
    Boil_Time = 60;

    //Adjunct Names
    strcpy (Adjunct_Name_1,"Mosaic 20g");
    strcpy (Adjunct_Name_2,"Fuggle 40g");
    strcpy (Adjunct_Name_3,"Hallertau 10g");
    strcpy (Adjunct_Name_4,"Irish Moss 5g");
    strcpy (Adjunct_Name_5,"Kent Golding 30g");
    strcpy (Adjunct_Name_6,"");
    strcpy (Adjunct_Name_7,"");
    strcpy (Adjunct_Name_8,"");

    //minutes
    Adjunct_Time_1 = 60;    //@x minutes remaining in boil
    Adjunct_Time_2 = 20;
    Adjunct_Time_3 = 20;
    Adjunct_Time_4 = 10;
    Adjunct_Time_5 = 0;
    Adjunct_Time_6 = 0;
    Adjunct_Time_7 = 0;
    Adjunct_Time_8 = 0;

    //*********COOLING*********

    //Deg C
    Cooling_Temp = 25;


    //Notes
    strcpy (Notes,"Big loooong string o' notes");


    //*********Calculation Variables*********
    Total_Brew_Volume = (Mash_Water_Volume + Sparge_Water_Volume - Grain_Weight);
 
}