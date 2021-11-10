void ActiveRecipe (int grainweight, int mashwatervol, int striketemp, int flowrate,
    int mashtime1, int mashtime2, int mashtime3, int mashtime4, int mashtime5,
    int mashtemp1, int mashtemp2, int mashtemp3, int mashtemp4, int mashtemp5,
    int spargewatervol, int spargetemp, int boiltime,
    char *adjunctname1, char *adjunctname2, char *adjunctname3, char *adjunctname4, char *adjunctname5,
    int adjuncttime1, int adjuncttime2, int adjuncttime3, int adjuncttime4, int adjuncttime5,
    int cooltemp);

//Control Variables, required for recipe

//*********MASH*********

//KGs
float Grain_Weight;

//Liters
float Mash_Water_Volume;

//Deg C
int Strike_Temp;

//0-100% of valve open
int Mash_Flow_Rate;

//minutes
int Mash_Time_1;
int Mash_Time_2;
int Mash_Time_3;
int Mash_Time_4;
int Mash_Time_5;

//Deg C
int Mash_Temp_1;
int Mash_Temp_2;
int Mash_Temp_3;
int Mash_Temp_4;
int Mash_Temp_5;


//*********SPARGE*********

//Liters
float Sparge_Water_Volume; 

//Deg C
int Sparge_Temp;

//*********BOIL*********

//Minutes
int Boil_Time;

//Adjunct Names
char Adjunct_Name_1[40];
char Adjunct_Name_2[40];
char Adjunct_Name_3[40];
char Adjunct_Name_4[40];
char Adjunct_Name_5[40];

//minutes
int Adjunct_Time_1;
int Adjunct_Time_2;
int Adjunct_Time_3;
int Adjunct_Time_4;
int Adjunct_Time_5;

//*********COOLING*********

//Deg C
int Cooling_Temp;


//Calculation Variable (not user defined)
float Total_Brew_Volume;