//void Auto_Run (struct Auto_Run_Controls Auto_Run_Task);

struct Auto_Run_Controls
{
    int Valve1, Valve2, Valve3, FlowCtrl, Target_Temp, Target_Sensor, Pump, Heater; 
    float Target_Volume; 
    int Target_Time;
};

void Auto_Run (struct Auto_Run_Controls *Auto_Run_Task);