void EquipConfig(void);

int Main_Config;    //1 = full, 2 = BIAB + Sparge, 3 = BIAB
int Kettle_Volume; //0-127
int Mash_Tun_Volume; //0-127
int Sparge_Connection; //1 = tank, 0 = tap
int Cooling_Rqd;  // 0 = n, 1 = y
int Cooling_Method; // 0 = pumped water, 1 = pumped wort
int Transfer_Method; //0 = manual, 1 = pumped
int Heating_Method; //0 = boiler, 1 = RIMS
int Safety_Margin; //xL of safety margin for kettle and mash tun (Hardcoded)