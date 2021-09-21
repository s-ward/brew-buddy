#include <stdio.h>
#include "interrupts.h"


#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <string.h>


#include "led.h"



//flow meter
#define FM_LARGE 23
#define FM_SMALL 21



#define GPIO_OUTPUT_LED    2 //set which gpio are which output
#define GPIO_OUTPUT_LED_2 18
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_LED) | (1ULL<<GPIO_OUTPUT_LED_2)) //bitmask output pins

#define GPIO_INPUT_BUTTON     13 //button
#define GPIO_INPUT_IO_1     5
//#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_BUTTON) | (1ULL<<GPIO_INPUT_IO_1))
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_BUTTON) | (1ULL<<FM_LARGE)) 


#define ESP_INTR_FLAG_DEFAULT 0



//flow meter
//static uint32_t currentTime = 0;
//static uint32_t previousTime = 0;
int interval = 1000;
float calibration_big = 7;
float calibration_small = 4.5;
volatile uint16_t pulseCount = 0;
uint16_t pulse1Second = 0;
float flowRate;
uint32_t flowMl;
uint32_t totalMl;





static xQueueHandle gpio_evt_queue = NULL; //declare queue
static const uint32_t debounceTime = 10;




//flow meter
static void IRAM_ATTR pulseCountAdd(){
    pulseCount++;
}




static void IRAM_ATTR button_isr_handler(void* arg) {


    uint32_t gpio_num = (uint32_t) arg;
    
    static uint32_t lastInterrupt = 0; //start counter for interrupt debounce
    uint32_t interruptTime = xTaskGetTickCount(); //get interrupt time

    if (interruptTime - lastInterrupt > debounceTime) {
       //xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL); //if debounce time has passed, send interrupt
       xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL); //if debounce time has passed, send interrupt
        
    }
    lastInterrupt = interruptTime; //set last interrupt to current time
}


//static xQueueHandle gpio_evt_queue = NULL; //declare queue

static void gpio_task_example(void *arg) {
    //uint32_t gpio_num;
    char gpio_num[50];
   // printf(gpio_num);
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &gpio_num, portMAX_DELAY)) {
            //do task
            //printf("GPIO[%s] intr, val: %s\n", gpio_num, gpio_get_level(gpio_num));
            printf("GPIO[%s] intr, val: %s\n", gpio_num, gpio_num);
            //toggle_led();
            led_toggle();
            

        }
    }
}


// struct Interrupts {
//         int gpio_num;
//         char message[50];
        
// };

// void button(void* string){
    void button(struct Interrupts *intr){

   //struct Interrupts int1* =   (Interrupts*) string;

   //int int1int  int1.gpio_num;

   //int gg =  int1.gpio_num;

    //char st = string;
     //strcpy(int1.message, "test mesage");

    //int intone = intr->gpio_num;
    char message[50];
    strcpy(message, intr->message);
    //char mss[] = strcpy(message, intr->message);

    //gpio_isr_handler_add(GPIO_INPUT_BUTTON, gpio_isr_handler, (void*) GPIO_INPUT_BUTTON);
    gpio_isr_handler_add(GPIO_INPUT_BUTTON, button_isr_handler, (void*) message);

    //printf(message);


}

void flowM() {

    gpio_isr_handler_add(FM_LARGE, pulseCountAdd, NULL);

}


//pass in 
//void getFlowRate(void) {
void getFlowRate(uint32_t timeDifference) { //timedifference = currenttime - previous time, around 1000millis
    pulse1Second = pulseCount;
    pulseCount = 0;

    //flowRate = ((1000.0 / (currentTime - previousTime)) * pulse1Second) / calibration_big;
    flowRate = ((1000.0 / (timeDifference)) * pulse1Second) / calibration_big;
    //previousTime = getTime.

    flowMl = (flowRate / 60) * 1000; //ml per minute

    totalMl += flowMl;

    //print n shit
    printf("flow rate: %d ml/m, total ml: %d ml\n", flowMl, totalMl);
}



void interrupts_config(void){

   gpio_config_t io_config;

    //set up outputs
    io_config.intr_type = GPIO_PIN_INTR_DISABLE;
    io_config.mode = GPIO_MODE_INPUT_OUTPUT; //input_output allows level to be read, otherwise just output
    io_config.pin_bit_mask = GPIO_OUTPUT_PIN_SEL; // bitmask from defines
    io_config.pull_down_en = 0; //disable pulldown
    io_config.pull_up_en = 0; //disable pullup
    gpio_config(&io_config); //configure with above settings

    //set up inputs
    io_config.intr_type = GPIO_PIN_INTR_POSEDGE; // interrupt on positive edge
    io_config.pin_bit_mask = GPIO_INPUT_PIN_SEL; //bitmask from defines
    io_config.mode = GPIO_MODE_INPUT; 
    io_config.pull_down_en = 0; //disable pulldown
    io_config.pull_up_en = 1; //enable pullup
    gpio_config(&io_config);


    //flow mete;
    //volatile uint16_t pulseCount = 0;
    //float flowRate = 0;
    //uint32_t flowMl = 0;
    //uint32_t totalMl = 0;


    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t)); //create queue
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL); // start task
    
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    //add handlers for each input here
    //gpio_isr_handler_add(GPIO_INPUT_BUTTON, gpio_isr_handler, (void*) GPIO_INPUT_BUTTON);
    //gpio_isr_handler_add(GPIO_INPUT_BUTTON, gpio_isr_handler, (void*) message);

    //printf(message);


}