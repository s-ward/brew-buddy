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

//Variables for calculations
float Kettle_Safe;
float Mash_Displacement;
float Mash_Safe;
float Mash_Exceeded;
