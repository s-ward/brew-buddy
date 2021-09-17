struct Interrupts {
        int gpio_num;
        char message[50];
        
};

void button(struct Interrupts *intr);
void flowM(void);
void interrupts_config(void);
void getFlowRate(uint32_t timeDifference);

