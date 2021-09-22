//void func(void);


struct brewery_setup_struct {

    int kettle_volume;
    int mash_tun_volume;
    bool pumped_transfer;
    char units[10];
    int lead_time;
    char heating_method[20];
    char cooling_method[20];
};

struct brewery_setup_struct brewery_setup;

//esp_err_t save_gpio_state(void);
//esp_err_t save_gpio_state(int gpio_num);
esp_err_t save_gpio_state(int gpio_num, int gpio_state);
esp_err_t save_brewery_setup(int kettlevolume, int mashtunvolume, bool pumpedtransfer,
    char *units, int leadtime, char *heatingmethod, char*coolingmethod);
//esp_err_t load_gpio_state(void);
esp_err_t load_gpio_state(int gpio_num);
esp_err_t load_brewery_setup(void);
void nvs_config(void);

void update_brewery_setup(int kettlevolume, int mashtunvolume, bool pumpedtransfer,
    char *units, int leadtime, char *heatingmethod, char *coolingmethod);