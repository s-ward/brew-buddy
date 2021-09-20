void ActiveRecipe(void);

//Passive Variables
//These may be required to pass to the webserver? otherwise can be removed 

char Recipe_Name[40];

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
char Adjunct_Name_6[40];
char Adjunct_Name_7[40];
char Adjunct_Name_8[40];

//minutes
int Adjunct_Time_1;
int Adjunct_Time_2;
int Adjunct_Time_3;
int Adjunct_Time_4;
int Adjunct_Time_5;
int Adjunct_Time_6;
int Adjunct_Time_7;
int Adjunct_Time_8;

//*********COOLING*********

//Deg C
int Cooling_Temp;



//Notes
char Notes[1000];



//Calculation Variable (not user defined)
float Total_Brew_Volume;