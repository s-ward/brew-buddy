int Heater_PID (int Target_Temp, int Target_Sensor);

int Temp_Reached;
int Duty_Cycle;
float Actual_Temp;

struct PIDController{

	/* Controller gains */
	float Kp;
	float Ki;
	float Kd;

	/* Derivative low-pass filter time constant */
	float tau;

	/* Output limits */
	float limMin;
	float limMax;
	
	/* Integrator limits */
	float limMinInt;
	float limMaxInt;

	/* Sample time (in seconds) */
	float T;

	/* Controller "memory" */
	float integrator;
	float prevError;			/* Required for integrator */
	float differentiator;
	float prevMeasurement;		/* Required for differentiator */

	/* Controller output */
	float out;

} PIDController;

struct PIDController TempSensor1;
struct PIDController TempSensor2;
struct PIDController TempSensor3;
struct PIDController FlowHeat;

void  PIDController_Init(void);
float PIDController_Update(struct PIDController *pid, float setpoint, float measurement);