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

int Defult_Setting;
int Wait;
int Valves_Set;
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

int Step_Active;

//Variables for calculations
float Kettle_Safe;
float Mash_Displacement;
float Mash_Safe;
float Mash_Exceeded;

//Control Variables
static int On = 1;
static int Off = 0;