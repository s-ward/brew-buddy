void ActiveRecipe(void);

//Passive Variables
//These may be required to pass to the webserver? otherwise can be removed 

char Recipe_Name[40];
char Beer_Style[40];
char Brew_Method[40];
float Batch_Size;
float ABV;
float SG;
float FG;

//KGs
float Grain_Weight_1;
float Grain_Weight_2;
float Grain_Weight_3;
float Grain_Weight_4;
float Grain_Weight_5;
float Grain_Weight_6;
float Grain_Weight_7;
float Grain_Weight_8;

//Grain Names
char Grain_Name_1[40];
char Grain_Name_2[40];
char Grain_Name_3[40];
char Grain_Name_4[40];
char Grain_Name_5[40];
char Grain_Name_6[40];
char Grain_Name_7[40];
char Grain_Name_8[40];

//*********MASH*********

//Liters
float Mash_Water_Volume;

//Deg C
int Strike_Temp;

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

//0-100% of valve open
int Mash_Flow_1;
int Mash_Flow_2;
int Mash_Flow_3;
int Mash_Flow_4;
int Mash_Flow_5;


//*********SPARGE*********
//Liters
float Sparge_Water_Volume; 

//Deg C
int Sparge_Temp;

//*********BOIL*********

//Adjunct Names
char Adjunct_Name_1[20];
char Adjunct_Name_2[20];
char Adjunct_Name_3[20];
char Adjunct_Name_4[20];
char Adjunct_Name_5[20];
char Adjunct_Name_6[20];
char Adjunct_Name_7[20];
char Adjunct_Name_8[20];

//grams
int Adjunct_Qty_1;
int Adjunct_Qty_2;
int Adjunct_Qty_3;
int Adjunct_Qty_4;
int Adjunct_Qty_5;
int Adjunct_Qty_6;
int Adjunct_Qty_7;
int Adjunct_Qty_8;

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


//Calculation Variable
float Total_Grain_Weight;
float Total_Brew_Volume;