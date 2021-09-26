
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

/* 
convert angle to pulse width
angle - angle to move servo to, can change this to just take open or closed
servo - the valve to move (valve_tap_in, valve_sparge_in, valve_sparge_out) 
*/
void valve_set_position (int angle, servo_params* servo) {

    float AnglePercent = angle;
    angle = (AnglePercent/100)*90;     //Convert % to 0-90 deg

    if (!servo->internal)  //If valve set to external
        angle = 270 - angle;    // Flow control setting for externally set valve

    printf("%s - Angle: %d\n", servo->name, angle);

    mcpwm_set_duty_in_us(servo->mcpwm_num, servo->timer_num, servo->gen, servo_per_degree_init(angle));
}

/*
  convert duty % back in to angle
*/
uint32_t valve_get_position (servo_params* servo) {

    return duty_percent_to_angle(mcpwm_get_duty(servo->mcpwm_num, servo->timer_num, servo->gen));
}

void servo_init(void) {

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