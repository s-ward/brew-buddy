//void func(void);

struct brewery_setup_struct
{
    int main_config;
    int external_connection;

    int kettle_volume;
    int mash_tun_volume;
    int pumped_transfer;
    char units[10];
    int lead_time;
    char heating_method[20]; // heatingmethods: ['RIMS', 'Element'],
    char cooling_method[20]; // coolingmethods: ['None', 'Wort Recirc', 'Water Recirc']
    int auto_fill;
    int safety_margin;
};

struct brewery_setup_struct brewery_setup;

//currently fixed max length for recipe list but look at malloc/realloc
char esprecipelist[20000];

void update_recipe_list(char *recipelist);
char *get_esp_recipe_list(void);

//esp_err_t save_gpio_state(void);
//esp_err_t save_gpio_state(int gpio_num);
esp_err_t save_gpio_state(int gpio_num, int gpio_state);
esp_err_t save_brewery_setup(int kettlevolume, int mashtunvolume, bool pumpedtransfer,
                             char *units, int leadtime, char *heatingmethod, char *coolingmethod);
//esp_err_t load_gpio_state(void);
esp_err_t load_gpio_state(int gpio_num);
esp_err_t load_brewery_setup(void);
esp_err_t load_recipe_list(void);
esp_err_t save_recipe_list(char *recipelist);

void nvs_config(void);

void update_brewery_setup(int kettlevolume, int mashtunvolume, bool pumpedtransfer,
                          char *units, int leadtime, char *heatingmethod, char *coolingmethod);

// placeholders for Boonies state code


// Pause_In - set to 1 to pause, 0 to resume.


// brew progress

void timertesttask(void);

int Status;
// 0 is idle (blank), 1 is running (green, play), 2 is paused (yellow, pause), 3 is adjunct req (orange, not paused), 
// 4 is user int req (red, paused, stop square.)

// char *User_Int_Required; // message to be sent
// int User_Int_Required_Int; // 1 for required, 0 for normal - pause
// char *User_Adjunct_Required; // message to be sent
// int User_Adjunct_Required_Int; // 1 for required, 0 for normal - don't pause

// void Manual_Config(int valve1, int valve2, int valve3, int targetflow1, int targetflow2, int targetflow3, int pump, 
//     int heater, int heater_power, int temp_sensor, int target_temp, int time, int volume);

