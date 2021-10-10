#include <stdio.h>
#include "HeaterPID.h"


int Heater_Duty_Cycle;

void PIDController_Init(struct PIDController *pid) {

	/* Clear and set controller variables */


    //SENSOR 1
    TempSensor1.Kp = 0.0f;
    TempSensor1.Ki = 0.0f;
    TempSensor1.Kd = 0.0f;
    TempSensor1.tau = 0.0f;
    TempSensor1.limMin = 0.0f;       //Min O/P value
    TempSensor1.limMax = 100.0f;     //Max O/P value           
    TempSensor1.limMinInt = 5.0f;        //Integrator limits
    TempSensor1.limMaxInt = -5.0f;
    TempSensor1.T = 1.0f;         //1 - sec update time

    TempSensor1.integrator = 0.0f;
    TempSensor1.prevError = 0.0f;
    TempSensor1.differentiator = 0.0f;
    TempSensor1.prevMeasurement = 0.0f;
    TempSensor1.out = 0.0f;

    //SENSOR 2


	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;

	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;

}



int Heater_PID (int Target_Temp, int Target_Sensor)
{

    Heater_Duty_Cycle = 10;         //manual value atm, proper PID function will update this automatically
    //printf("%d\n", Heater_Duty_Cycle);

    //if ((Target_Temp-1) <= Actual_Temp <= (Target_Temp+1))
        Temp_Reached = 1;

    //else
        //Temp_Reached = 0;

    return (Heater_Duty_Cycle);
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