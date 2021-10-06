void EquipConfig(void);

int Main_Config;    //1 = full, 2 = BIAB + Sparge, 3 = BIAB
int Kettle_Volume; //0-127
int Mash_Tun_Volume; //0-127
int External_Connection; //0 = tank, 1 = tap
int Cooling_Rqd;  // 0 = n, 1 = y
int Cooling_Method; // 0 = pumped water, 1 = pumped wort
int Transfer_Method; //0 = manual, 1 = pumped
int Heating_Method; //0 = boiler, 1 = RIMS

int Auto_Fill; //0 = manual, 1 = autofill kettle for mash

int Safety_Margin; //xL of safety margin for kettle and mash tun (Hardcoded)

void Manual_Config(int valve1, int valve2, int valve3, int targetflow1, int targetflow2, int targetflow3, int pump, 
    int heater, int heater_power, int temp_sensor, int target_temp, int time, int volume);

int Man_Valve1;
int Man_Valve2;
int Man_Valve3;

int Man_Valve1_Flow;
int Man_Valve2_Flow;        //ignore
int Man_Valve3_Flow;        //ignore

int Man_Pump;

int Man_Heater;
int Man_Heater_Power;

int Man_Sensor;
int Man_Temp;

int Man_Time;

int Man_Volume;



