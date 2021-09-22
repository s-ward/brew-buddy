
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "servo.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"

#include "esp_attr.h"

//values from servo datasheet
#define SERVO_MIN_PULSEWIDTH 500 //Minimum pulse width microsecond for servo
#define SERVO_MAX_PULSEWIDTH 2500 //Maximum pulse width in microsecond for servo
#define SERVO_MAX_DEGREE 270

//Convert angles in degrees to angles in PWM for servo
uint32_t servo_per_degree_init(uint32_t degree_of_rotation)
    {
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE)));
    return cal_pulsewidth;
    }

//convert duty cycle % from get_duty() to angle in degrees
uint32_t duty_percent_to_angle (float duty_percent) {

    float angle = 0.0;
    angle = ((duty_percent -2.5) / 10) * 270;
    return round(angle);
  
}

//Potentially have one of these converters above for each servo??? is this needed??
/*uint32_t servo_per_degree_TI_valve_init(uint32_t degree_of_rotation_TI_valve)
{
    uint32_t TI_valve_pulse_width_cal = 0;
    TI_valve_pulse_width_cal = (MOTOR_MIN_PULSE_WIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation_TI_valve)) / (SERVO_TAP_IN_MAX_DEGREE)));
    
    printf("Tap In Valve value %d\n",TI_valve_pulse_width_cal);
    return TI_valve_pulse_width_cal;
}
*/

//Write PWM values to all the Servos
/* void mcpwm_servo_control(int angle_TI,int angle_SI,int angle_SO)
    { 
        PWM_TI = servo_per_degree_init(angle_TI_valve);
        PWM_SI = servo_per_degree_init(angle_SI_valve);
        PWM_SO = servo_per_degree_init(angle_SO_valve);
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, PWM_TI);
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, PWM_SI);
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, PWM_SO);
    }
 */

//Use this function whenever you want the servo in the "open" position
/* void open_valve()
{
	uint32_t open = 0;
	uint32_t angle;

	printf("Angle of rotation: %d\n", open);
	angle = servo_per_degree_init(open);

	printf("pulse width: %dus\n", angle);
	mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
	vTaskDelay(10);    
} */
//Use this function whenever you want the servo in the "closed" position
/* void close_valve()
{
	uint32_t close = SERVO_MAX_DEGREE;
    uint32_t angle;

    printf("Angle of rotation: %d\n", close);
    angle = servo_per_degree_init(close);

    printf("pulse width: %dus\n", angle);
	mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
    vTaskDelay(10);    

} */

/* 
convert angle to pulse width
angle - angle to move servo to, can change this to just take open or closed
servo - the valve to move (valve_tap_in, valve_sparge_in, valve_sparge_out) 
*/
void valve_set_position (int angle, servo_params* servo) {

    mcpwm_set_duty_in_us(servo->mcpwm_num, servo->timer_num, servo->gen, servo_per_degree_init(angle));
}

/*
  convert duty % back in to angle
*/
uint32_t valve_get_position (servo_params* servo) {

    return duty_percent_to_angle(mcpwm_get_duty(servo->mcpwm_num, servo->timer_num, servo->gen));
}

void init_servo_params() {

    static servo_params valve_tap_in = {MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, VALVE_TAP_IN, MCPWM_OPR_A, "Valve Tap In"};
    static servo_params valve_sparge_in = {MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0B, VALVE_SPARGE_IN, MCPWM_OPR_B, "Valve Sparge In"};
    static servo_params valve_sparge_out = {MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM1A, VALVE_SPARGE_OUT, MCPWM_OPR_A, "Valve Sparge Out"};
}



void servo_init(void) {

   // init_servo_params();

    printf("%s Pointer Address: %p\n", valve_tap_in.name, &valve_tap_in);
    printf("%s Pointer Address: %p\n", valve_sparge_in.name, &valve_sparge_in);
    printf("%s Pointer Address: %p\n", valve_sparge_out.name, &valve_sparge_out);

    //initialise each servo valve
    mcpwm_gpio_init(valve_tap_in.mcpwm_num, valve_tap_in.io_signal, valve_tap_in.gpio_num); 
    mcpwm_gpio_init(valve_sparge_in.mcpwm_num, valve_sparge_in.io_signal, valve_sparge_in.gpio_num);  
    mcpwm_gpio_init(valve_sparge_out.mcpwm_num, valve_sparge_out.io_signal, valve_sparge_out.gpio_num);   

    //Configure pwm
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
        
    //Assign the configurations to MCPWM_UNIT - doesn't actually need to be for each servo just each timer (first two the same)
    mcpwm_init(valve_tap_in.mcpwm_num, valve_tap_in.timer_num, &pwm_config); 
    mcpwm_init(valve_sparge_in.mcpwm_num, valve_sparge_in.timer_num, &pwm_config); 
    mcpwm_init(valve_sparge_out.mcpwm_num, valve_sparge_out.timer_num, &pwm_config); 

}



//Below is possible coding for opening and closing of valves
/* if(valve_state == open) {
				valve_state = close;
				open_valve();
				printf("Valve Opened\n");
			} else {
				printf("Already in open state\n");
            }


if(valve_state == close) {
				valve_state = open;
				close_valve();
				printf("Valve Closed\n");
			} else {
				printf("Already in close state\n");
            }

*/ 



//Below is some other ideas that could be used 

/*
 
void task_valve_open(void *pvParameters) {
   
printf("Opening the valve\n");

   // actuate servo 1
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, SERVO_MAX_PULSEWIDTH);

    // delay servo 2 to avoid drawing too much current
    vTaskDelay(SERVO_DELAY / portTICK_PERIOD_MS);

    // actuate servo 2
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, SERVO_MAX_PULSEWIDTH);

    // continue opening
    vTaskDelay(SERVO_DURATION / portTICK_PERIOD_MS);
    
 printf("Valve opened\n");

    busy = false;

    vTaskDelete(NULL);
}

void task_valve_close(void *pvParameters) {
    
    printf("Closing the valve\n");

    // actuate servo 2
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, SERVO_MIN_PULSEWIDTH);

    // delay servo 1
    vTaskDelay(SERVO_DELAY / portTICK_PERIOD_MS);

    // actuate servo 1
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, SERVO_MIN_PULSEWIDTH);

    printf("Valve closed\n");


      xTaskCreate(task_valve_control, "valve_control", 4096, NULL, 10, NULL);





void moveServo(int control_valve, int gpio_num)
{
  void loop() {
 
  //Open Valve_Tap_In
  moveServo(open_valve, Valve_Tap_In);
  //Close Valve_Tap_In
  moveServo(close_valve, Valve_Tap_In);  
    
  //Open Valve_Sparge_In
  moveServo(open_valve, Valve_Sparge_In);
  //Close Valve_Sparge_In
  moveServo(close_valve, Valve_Sparge_In);

  //Open Valve_Sparge_Out
  moveMotor(open_valve, Valve_Sparge_Out);
 //Close Valve_Sparge_Out
  moveServo(close_valve, Valve_Sparge_Out);

  
}


      */