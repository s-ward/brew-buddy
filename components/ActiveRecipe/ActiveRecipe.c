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

    strcpy (Recipe_Name, "Test recipe");
    strcpy (Beer_Style, "Do not Drink");
    strcpy (Brew_Method, "All Grain, Full Config"); //Specify if BIAB etc
    ABV = 99.9;
    SG = 1.082;
    FG = 1.01;

    //Control Variables, required for recipe
    
    //KGs
    Grain_Weight_1 = 0.6;
    Grain_Weight_2 = 2;
    Grain_Weight_3 = 0;
    Grain_Weight_4 = 4;
    Grain_Weight_5 = 0;
    Grain_Weight_6 = 0;
    Grain_Weight_7 = 0;
    Grain_Weight_8 = 0;

    //Grain Names
    strcpy (Grain_Name_1, "Crystal");
    strcpy (Grain_Name_2, "Roast");
    strcpy (Grain_Name_3, "");
    strcpy (Grain_Name_4, "2-Row");
    strcpy (Grain_Name_5, "");
    strcpy (Grain_Name_6, "");
    strcpy (Grain_Name_7, "");
    strcpy (Grain_Name_8, "");

    //*********MASH*********

    //Liters
    Mash_Water_Volume = 30;
    //Deg C
    Strike_Temp = 75;

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

    //0-100% of valve open
    Mash_Flow_1 = 50;
    Mash_Flow_2 = 50;
    Mash_Flow_3 = 50;
    Mash_Flow_4 = 0;
    Mash_Flow_5 = 0;

    //*********SPARGE*********

    //Liters
    Sparge_Water_Volume = 30; 

    //Deg C
    Sparge_Temp = 75;
 
    //*********BOIL*********

    //Adjunct Names
    strcpy (Adjunct_Name_1,"Mosaic");
    strcpy (Adjunct_Name_2,"Fuggle");
    strcpy (Adjunct_Name_3,"Hallertau");
    strcpy (Adjunct_Name_4,"Irish Moss");
    strcpy (Adjunct_Name_5,"Kent Golding");
    strcpy (Adjunct_Name_6,"");
    strcpy (Adjunct_Name_7,"");
    strcpy (Adjunct_Name_8,"");

    //grams
    Adjunct_Qty_1 = 50;
    Adjunct_Qty_2 = 22;
    Adjunct_Qty_3 = 96;
    Adjunct_Qty_4 = 5;
    Adjunct_Qty_5 = 60;
    Adjunct_Qty_6 = 0;
    Adjunct_Qty_7 = 0;
    Adjunct_Qty_8 = 0;

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



    //Passive Ingredients

    //yeast name
    strcpy (Yeast,"London II");

    //Mash water chemistry (mg)
    Mash_Gypsum = 12;
    Mash_Calcium_Chloride  = 0; 
    Mash_Epsom_Salt  = 0; 
    Mash_Magnesium_Chloride  = 0; 
    Mash_Canning_Salt = 0; 
    Mash_Baking_Soda = 0;
    Mash_Chalk = 0; 
    Mash_Pickling_Lime = 0; 
    Mash_Acid = 0;  

    //Sparge water chemistry (mg)
    Sparge_Gypsum = 12;
    Sparge_Calcium_Chloride  = 0; 
    Sparge_Epsom_Salt  = 0; 
    Sparge_Magnesium_Chloride  = 0; 
    Sparge_Canning_Salt = 0; 
    Sparge_Baking_Soda = 0;
    Sparge_Chalk = 0; 
    Sparge_Pickling_Lime = 0; 
    Sparge_Acid = 0;  



    //*********Calculation Variables*********
    Total_Grain_Weight = (Grain_Weight_1 + Grain_Weight_2 + Grain_Weight_3 + Grain_Weight_4 + Grain_Weight_5 + Grain_Weight_6 + Grain_Weight_7 + Grain_Weight_8);
    Total_Brew_Volume = (Mash_Water_Volume + Sparge_Water_Volume - Total_Grain_Weight);
 
}