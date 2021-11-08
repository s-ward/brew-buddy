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

#define GPIO_OUTPUT_LED 2 //set which gpio are which output
#define GPIO_OUTPUT_LED_2 18
#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_OUTPUT_LED) | (1ULL << GPIO_OUTPUT_LED_2)) //bitmask output pins

#define GPIO_INPUT_BUTTON 13 //button
#define GPIO_INPUT_IO_1 5
//#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_BUTTON) | (1ULL<<GPIO_INPUT_IO_1))
#define GPIO_INPUT_PIN_SEL ((1ULL << GPIO_INPUT_BUTTON) | (1ULL << FLOW_METER_GPIO_TAP_IN) | (1ULL << FLOW_METER_GPIO_SPARGE_IN) | (1ULL << FLOW_METER_GPIO_SPARGE_OUT))

#define ESP_INTR_FLAG_DEFAULT 0

#define FLOW_METER_GPIO_TAP_IN 23     // first flow meter - revise name
#define FLOW_METER_GPIO_SPARGE_IN 21  // second flow meter - revise name
#define FLOW_METER_GPIO_SPARGE_OUT 18 // third flow meter - revise name

//#define FLOW_METER_CAL_SMALL 7
#define FLOW_METER_CAL_LARGE 11

//#define FLOW_METER_INTERVAL 1000

//flow meter
//static uint32_t currentTime = 0;
//static uint32_t previousTime = 0;

float calibration_big = 7;
float calibration_small = 4.5;
volatile uint16_t pulseCount = 0;
uint16_t pulse1Second = 0;
float flowRate;
uint32_t flowMl;
uint32_t totalMl;

static uint32_t currentTime = 0;
static uint32_t previousTime = 0;
//int timeInterval = 1000; // interval time in milli seconds

static xQueueHandle gpio_evt_queue = NULL; //declare queue
static const uint32_t debounceTime = 10;

//flow meter -
static void IRAM_ATTR pulseCountAdd()
{
    pulseCount++;
}

// flow meter ISR
// could make task but seems pretty lightweight, is this thread safe to use same ISR for 3 GPIO's
// yes should be thread safe because not modifying global, only modifying struct vars
// actually possibly not if there was another function that modified the same meters pulse count.
// ISRS are non locking apparently
// this is kinda fucked think I should change to three seperate ISR's
static void IRAM_ATTR pulse_count_add(struct FlowMeter *flowMeter)
//static void IRAM_ATTR pulse_count_add(void* arg)
{
    //struct FlowMeter* flowMeter = arg;

    //static void IRAM_ATTR pulse_count_add(void *arg) {
    //  int flow = flowMeter->pulse_count++;
    //flowMeter->pulse_count = flow;
    //ets_printf("flow: %d\n", flowMeter->pulse_count);
    //struct FlowMeter *flowMeter = arg;
    flowMeter->pulse_count++;
    flowMeter->pulse_count_total++;

}

static void IRAM_ATTR button_isr_handler(void *arg)
{

    uint32_t gpio_num = (uint32_t)arg;

    static uint32_t lastInterrupt = 0;            //start counter for interrupt debounce
    uint32_t interruptTime = xTaskGetTickCount(); //get interrupt time

    if (interruptTime - lastInterrupt > debounceTime)
    {
        //xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL); //if debounce time has passed, send interrupt
        xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL); //if debounce time has passed, send interrupt
    }
    lastInterrupt = interruptTime; //set last interrupt to current time
}

//static xQueueHandle gpio_evt_queue = NULL; //declare queue

static void gpio_task_example(void *arg)
{
    //uint32_t gpio_num;
    char gpio_num[50];
    // printf(gpio_num);
    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &gpio_num, portMAX_DELAY))
        {
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
void button(struct Interrupts *intr)
{

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
    gpio_isr_handler_add(GPIO_INPUT_BUTTON, button_isr_handler, (void *)message);

    //printf(message);
}

int get_meter_flow_rate(struct FlowMeter *flowMeter)
{
    return flowMeter->flow_rate;
}
int get_meter_flow_total(struct FlowMeter *flowMeter)
{
    return flowMeter->flow_total;
}
void reset_meter_flow_total(struct FlowMeter *flowMeter)
{
    flowMeter->flow_total = 0;
}

void flowM()
{

    gpio_isr_handler_add(FM_LARGE, pulseCountAdd, NULL);
}

//pass in
//void getFlowRate(void) {
void getFlowRate(uint32_t timeDifference)
{ //timedifference = currenttime - previous time, around 1000millis
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

// void updateFlowMeter(struct FlowMeter *flowMeter, uint32_t timeDifference){
void updateFlowMeter(struct FlowMeter *flowMeter)
{

    uint32_t current_time = esp_timer_get_time() / 1000ULL;             // current time since boot in milliseconds
    uint32_t time_difference = current_time - flowMeter->previous_time; //around 1000millis
    flowMeter->previous_time = current_time;
    //printf("Flow Meter Time Difference: %d, \n", time_difference);
    //printf("Flow Meter Current Time: %d, \n", current_time);
    //printf("Flow Meter Pulse Count: %d, \n", flowMeter->pulse_count);

    flowMeter->pulse_count_1_second = flowMeter->pulse_count;
    flowMeter->pulse_count = 0;

    flowMeter->flow_rate_second = ((1000.0 / (time_difference)) * flowMeter->pulse_count_1_second) / flowMeter->calibration; // L/m
    
    // how many mL flowed in this 1 second
    //flowMeter->flow_rate = (flowMeter->flow_rate_second / 60) * 1000;
    flowMeter->flow_rate = flowMeter->flow_rate_second * 1000; // mL/m

    flowMeter->flow_total += (flowMeter->flow_rate / 60 ); // from above, add mL amount from this 1 second to total.
}

void initFlowMeters(void)
{

    flowMeterTapIn = (struct FlowMeter){FLOW_METER_GPIO_TAP_IN, 0, 0, FLOW_METER_CAL_LARGE, 0, 0, 0, 0, 0};
   // printf("%d Pointer Address inside init: %p\n", flowMeterTapIn.gpio_num, &flowMeterTapIn);
    flowMeterSpargeIn = (struct FlowMeter){FLOW_METER_GPIO_SPARGE_IN, 0, 0, FLOW_METER_CAL_LARGE, 0, 0, 0, 0, 0};
    flowMeterSpargeOut = (struct FlowMeter){FLOW_METER_GPIO_SPARGE_OUT, 0, 0, FLOW_METER_CAL_LARGE, 0, 0, 0, 0, 0};
   // static struct FlowMeter flowMeterSpargeOut = {FLOW_METER_GPIO_SPARGE_OUT, 0, 0, FLOW_METER_CAL_LARGE, 0, 0, 0, 0, 0};
}

void interrupts_config(void)
{

    initFlowMeters();

    printf("%d Pointer Address: %p\n", flowMeterTapIn.gpio_num, &flowMeterTapIn);
    printf("%d Pointer Address: %p\n", flowMeterSpargeIn.gpio_num, &flowMeterSpargeIn);
    printf("%d Pointer Address: %p\n", flowMeterSpargeOut.gpio_num, &flowMeterSpargeOut);

    gpio_config_t io_config;

    //set up outputs
    io_config.intr_type = GPIO_PIN_INTR_DISABLE;
    io_config.mode = GPIO_MODE_INPUT_OUTPUT;      //input_output allows level to be read, otherwise just output
    io_config.pin_bit_mask = GPIO_OUTPUT_PIN_SEL; // bitmask from defines
    io_config.pull_down_en = 0;                   //disable pulldown
    io_config.pull_up_en = 0;                     //disable pullup
    gpio_config(&io_config);                      //configure with above settings

    //set up inputs
    io_config.intr_type = GPIO_PIN_INTR_POSEDGE; // interrupt on positive edge
    io_config.pin_bit_mask = GPIO_INPUT_PIN_SEL; //bitmask from defines
    io_config.mode = GPIO_MODE_INPUT;
    io_config.pull_down_en = 0; //disable pulldown
    io_config.pull_up_en = 1;   //enable pullup
    gpio_config(&io_config);

    //flow meter
    //volatile uint16_t pulseCount = 0;
    //float flowRate = 0;
    //uint32_t flowMl = 0;
    //uint32_t totalMl = 0;

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));                       //create queue
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL); // start task

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    // flow meter interrupts
 //gpio_isr_handler_add(GPIO_INPUT_BUTTON, button_isr_handler, &flowMeterSpargeOut);
    gpio_isr_handler_add(FLOW_METER_GPIO_TAP_IN, pulse_count_add, &flowMeterTapIn);

    gpio_isr_handler_add(FLOW_METER_GPIO_SPARGE_IN, pulse_count_add, &flowMeterSpargeIn);
    gpio_isr_handler_add(FLOW_METER_GPIO_SPARGE_OUT, pulse_count_add, &flowMeterSpargeOut);

    //add handlers for each input here
    //gpio_isr_handler_add(GPIO_INPUT_BUTTON, gpio_isr_handler, (void*) GPIO_INPUT_BUTTON);
    //gpio_isr_handler_add(GPIO_INPUT_BUTTON, gpio_isr_handler, (void*) message);

    //printf(message);
}