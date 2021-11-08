
struct Instant_Heat_Controls
{
    int Instant_Temp; 
    float Instant_Volume;
};

void Instant_Heat (struct Instant_Heat_Controls *Temp_Flow);
void FlowPID(int Targ_Temp);