#ifndef SERVO_H
#define SERVO_H

#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h" 

//these values will need to be tuned
#define VALVE_OPEN 170 
#define VALVE_CLOSE 10

#define VALVE_TAP_IN 18
#define VALVE_SPARGE_IN 19
#define VALVE_SPARGE_OUT 21 

typedef struct {

    mcpwm_unit_t mcpwm_num; //MCPWM_UNIT;
    mcpwm_timer_t timer_num; //MCPWM_TIMER;
    mcpwm_io_signals_t io_signal; // MCPWM_SIGNAL
    int gpio_num;
    mcpwm_generator_t gen; 
    char name[20];

} servo_params;

//forward declare and define structs for each servo
static servo_params valve_tap_in = {MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, VALVE_TAP_IN, MCPWM_OPR_A, "Valve Tap In"};
static servo_params valve_sparge_in = {MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0B, VALVE_SPARGE_IN, MCPWM_OPR_B, "Valve Sparge In"};
static servo_params valve_sparge_out = {MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM1A, VALVE_SPARGE_OUT, MCPWM_OPR_A, "Valve Sparge Out"};

void valve_set_position (int angle, servo_params* servo);
uint32_t valve_get_position(servo_params* valve);

void servo_init(void);




// #define idle_valve 0 //valve remains in current positon??? Needed?
//define open_valve 1 //open valve
//#define close_valve 2 //close valve





// GPIO pin constants
//static uint32_t open_valve = SERVO_MAX_PULSEWIDTH;
//static uint32_t close_valve = SERVO_MIN_PULSEWIDTH;

/* // Read state from 
int control_valve; 

// Read valve state
int valve_state;

int angle_TI_valve, angle_SI_valve, angle_SO_valve; //angle of valves
int PWM_TI ,PWM_SI ,PWM_SO; //PWM Values
uint32_t count; 

void open_valve();

void close_valve();

void mcpwm_gpio_initialize();

uint32_t servo_per_degree_init(uint32_t degree_of_rotation);

void mcpwm_servo_control(int angle_TI_valve, int angle_SI_valve, int angle_SO_valve); */




#endif