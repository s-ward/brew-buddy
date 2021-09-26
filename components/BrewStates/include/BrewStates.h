void Brew_States (void);

void Passive (void);
void WPS (void);
void Clean (void);
void Manual (void);
void Safety_Check (void);
void Mash (void);
void Sparge (void);
void Boil (void);
void Cooling (void);
void Transfer (void);
void Pause (void);
void Cancel (void);

enum BrewStates {Passive_State, Test_State, WPS_State, Clean_State, Manual_State, Safety_Check_State, 
                  Mash_State, Sparge_State, Boil_State, Cooling_State, Transfer_State};
char BrewState;


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

int Current_Flow1;
int Current_Flow2;
int Current_Flow3;

char Stage[20];
char Step[30];
char Auto_Process[30];

int Step_Active;

//Variables for calculations
float Kettle_Safe;
float Mash_Displacement;
float Mash_Safe;
float Mash_Exceeded;

//Control Variables
static int On = 1;
static int Off = 0;