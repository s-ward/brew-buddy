//void func(void);


//esp_err_t save_gpio_state(void);
//esp_err_t save_gpio_state(int gpio_num);
esp_err_t save_gpio_state(int gpio_num, int gpio_state);
//esp_err_t load_gpio_state(void);
esp_err_t load_gpio_state(int gpio_num);
void nvs_config(void);