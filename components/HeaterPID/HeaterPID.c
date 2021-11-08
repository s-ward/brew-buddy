#include <stdio.h>
#include "HeaterPID.h"
#include "BrewStates.h"


void PIDController_Init(void) {

	/* Clear and set controller variables */

    //SENSOR 1
    TempSensor1.Kp = 20.0f;
    TempSensor1.Ki = 0.5f;
    TempSensor1.Kd = 0.25f;
    TempSensor1.tau = 0.02f;
    TempSensor1.limMin = 0.0f;       //Min O/P value
    TempSensor1.limMax = 100.0f;     //Max O/P value           
    TempSensor1.limMinInt = 5.0f;        //Integrator limits
    TempSensor1.limMaxInt = -5.0f;
    TempSensor1.T = 1.0f;         //1 - sec update time
    //Reset below
    TempSensor1.integrator = 0.0f;
    TempSensor1.prevError = 0.0f;
    TempSensor1.differentiator = 0.0f;
    TempSensor1.prevMeasurement = 0.0f;
    TempSensor1.out = 0.0f;

    //SENSOR 2
    TempSensor2.Kp = 1000.0f;
    TempSensor2.Ki = 0.1f;
    TempSensor2.Kd = 0.0f;
    TempSensor2.tau = 0.02f;
    TempSensor2.limMin = 0.0f;       //Min O/P value
    TempSensor2.limMax = 100.0f;     //Max O/P value           
    TempSensor2.limMinInt = 5.0f;        //Integrator limits
    TempSensor2.limMaxInt = -5.0f;
    TempSensor2.T =1.0f;         //1 - sec update time
    //Reset below
    TempSensor2.integrator = 0.0f;
    TempSensor2.prevError = 0.0f;
    TempSensor2.differentiator = 0.0f;
    TempSensor2.prevMeasurement = 0.0f;
    TempSensor2.out = 0.0f;

    //SENSOR 3
    TempSensor3.Kp = 2.0f;
    TempSensor3.Ki = 0.5f;
    TempSensor3.Kd = 0.25f;
    TempSensor3.tau = 0.02f;
    TempSensor3.limMin = 0.0f;       //Min O/P value
    TempSensor3.limMax = 100.0f;     //Max O/P value           
    TempSensor3.limMinInt = 5.0f;        //Integrator limits
    TempSensor3.limMaxInt = -5.0f;
    TempSensor3.T = 1.0f;         //1 - sec update time
    //Reset below
    TempSensor3.integrator = 0.0f;
    TempSensor3.prevError = 0.0f;
    TempSensor3.differentiator = 0.0f;
    TempSensor3.prevMeasurement = 0.0f;
    TempSensor3.out = 0.0f;

	//INSTANT HEAT FLOW CONTROL PID
    FlowHeat.Kp = 20.0f;
    FlowHeat.Ki = 0.5f;
    FlowHeat.Kd = 0.5f;
    FlowHeat.tau = 0.02f;
    FlowHeat.limMin = 0.0f;       //Min O/P value, don't want 0 flow
    FlowHeat.limMax = 70.0f;     //Max O/P value           
    FlowHeat.limMinInt = 5.0f;        //Integrator limits
    FlowHeat.limMaxInt = -5.0f;
    FlowHeat.T = 1.0f;         //1 - sec update time
    //Reset below
    FlowHeat.integrator = 0.0f;
    FlowHeat.prevError = 0.0f;
    FlowHeat.differentiator = 0.0f;
    FlowHeat.prevMeasurement = 0.0f;
    FlowHeat.out = 0.0f;
}



int Heater_PID (int Targ_Temp, int Targ_Sensor)
{
    if (Targ_Sensor == 1)
    {
        if (Temp1 < 120)
        {    
            Actual_Temp = Temp1;
            Duty_Cycle = PIDController_Update(&TempSensor1, Targ_Temp, Temp1);
        }
    }
    else if (Targ_Sensor == 2)
    {
        if (Temp2 < 120)
        {    
            Actual_Temp = Temp2;
            Duty_Cycle = PIDController_Update(&TempSensor2, Targ_Temp, Temp2);
        }
    }
    else if (Targ_Sensor == 3)
    {
        if (Temp3 < 120)
        {
            Actual_Temp = Temp3;
            Duty_Cycle = PIDController_Update(&TempSensor3, Targ_Temp, Temp3);
        }
    }
    else if (Targ_Sensor == 4)
    {
        if (Temp2 < 120)
        {
            Actual_Temp = Temp2;
            Duty_Cycle = PIDController_Update(&FlowHeat, Targ_Temp, Temp2);    //Instant heat flow control PID
        }
    }
    //Duty_Cycle = 10;         //manual value atm, proper PID function will update this automatically
    //printf("%d\n", Heater_Duty_Cycle);

    if (Targ_Temp <= Actual_Temp)
    {
        Temp_Count++;
        if (Temp_Count == 3)
        {
            Temp_Reached = 1;
            printf("Temp Reached!!!\n");
            Temp_Count = 0;
        }
    }
    else
    {
        Temp_Reached = 0;
        Temp_Count = 0;
    }
    Current_Temp = Actual_Temp; // Var for app
    sprintf(Target_Temperature, "%d", Targ_Temp);

    printf("Heater Duty Cycle: %d\n", Duty_Cycle);
    return (Duty_Cycle);
}



float PIDController_Update(struct PIDController *pid, float setpoint, float measurement) {

	/*
	* Error signal 
	*/
    float error = setpoint - measurement;


	/*
	* Proportional
	*/
    float proportional = pid->Kp * error;


	/*
	* Integral
	*/
    pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

	/* Anti-wind-up via integrator clamping */
    if (pid->integrator > pid->limMaxInt) {

        pid->integrator = pid->limMaxInt;

    } else if (pid->integrator < pid->limMinInt) {

        pid->integrator = pid->limMinInt;

    }


	/*
	* Derivative (band-limited differentiator)
	*/
		
    pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prevMeasurement)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * pid->tau - pid->T) * pid->differentiator)
                        / (2.0f * pid->tau + pid->T);


	/*
	* Compute output and apply limits
	*/
    pid->out = proportional + pid->integrator + pid->differentiator;

    if (pid->out > pid->limMax) {

        pid->out = pid->limMax;

    } else if (pid->out < pid->limMin) {

        pid->out = pid->limMin;

    }

	/* Store error and measurement for later use */
    pid->prevError       = error;
    pid->prevMeasurement = measurement;

	/* Return controller output */
    return pid->out;

}