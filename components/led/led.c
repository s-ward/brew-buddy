#include <stdio.h>
#include "led.h"

//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
#include "driver/gpio.h"
//#include "sdkconfig.h"
//#include "nvs_flash.h"

//#include "nvs_blob.h"





//#define BLINK_GPIO CONFIG_BLINK_GPIO
//#define GPIO_LED 12
 

// change this to take in GPIO num
void led_config(void) {

        gpio_pad_select_gpio(GPIO_LED);
        gpio_set_direction(GPIO_LED, GPIO_MODE_INPUT_OUTPUT);
        //load state

}

//take in gpio num, or have led component for each led e.g. this is WPS LED
void led_toggle(void) {

    int led_state = gpio_get_level(GPIO_LED);
    gpio_set_level(GPIO_LED, !led_state);
    printf("Toggle LED\n");
    //save state
    //save_gpio_state();

}

//take in gpio num, return state
int led_get_state(int gpio_num){
    int led_state = gpio_get_level(gpio_num);
    printf("LED: %d State: %d\n", gpio_num, led_state);
    return led_state;
}