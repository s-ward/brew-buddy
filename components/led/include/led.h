//#define BLINK_GPIO 2
#define GPIO_LED 12  

void led_config(void);
void led_toggle(void);
int led_get_state(int gpio_num);
