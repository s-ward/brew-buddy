void Brew_States (void);

void Passive (void);
void WPS (void);
void Clean (void);
void Manual (void);
void Manual_User_Input(void);
void Safety_Check (void);
void Mash (void);
void Sparge (void);
void Boil (void);
void Cooling (void);
void Transfer (void);
void Pause (void);
void Cancel (void);

enum BrewStates {Passive_State, WPS_State, Clean_State, Manual_State, Safety_Check_State, 
                  Mash_State, Sparge_State, Boil_State, Cooling_State, Transfer_State} BrewState;

enum ManStates {Full_Man, Manual_Timer, Manual_Volume, Manual_Instant_Heat} ManState;

int WPS_In, Clean_In, Manual_In, Pause_In, Cancel_In, Brew_In;

int Defult_Setting;
int Wait;
int Volume_Reached;
int Timer;
int Minutes_Remaining;
int Seconds_Remaining;
int Absolute_Seconds_Remaining;
int Stage_complete;
int Auto_PID;               //Automatic 1, or manual 0
int PWM_En;
int Count_Update;
int PWM_State;
int Paused;
int Pump_State;
int Heater_State;
int Manual_Duty;
int Cold_Rinse;
int FlowPID_En;
int Man_EN;

int Current_Flow1;
int Current_Flow2;
int Current_Flow3;

int Valve1_State;
int Valve2_State;
int Valve3_State;

char Stage[20];
char Step[30];
char Auto_Process[30];

char User_Int_Required[200];
int User_Int_Rqd;

char User_Adjunct_Reruired[50];
int User_Adjunct_Rqd;

int Step_Active;

//Variables for calculations
float Kettle_Safe;
float Mash_Displacement;
float Mash_Safe;
float Mash_Exceeded;

//Control Variables
static int On = 1;
static int Off = 0;