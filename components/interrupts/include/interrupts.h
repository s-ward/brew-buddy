

struct Interrupts
{
    int gpio_num;
    char message[50];
};

struct FlowMeter
{
    uint32_t gpio_num;
    uint32_t flow_rate; // mL per minute
    uint32_t flow_total;

    //int interval = 1000;
    float calibration;
    float flow_rate_second;

    volatile uint16_t pulse_count;

    uint16_t pulse_count_1_second;

    uint32_t previous_time;
    //static uint32_t current_time;
    volatile uint16_t pulse_count_total;
};

struct FlowMeter flowMeterTapIn;
struct FlowMeter flowMeterSpargeIn;
struct FlowMeter flowMeterSpargeOut;

//static struct FlowMeter flowMeterTapIn = {FLOW_METER_GPIO_TAP_IN, 0, 0, FLOW_METER_CAL_LARGE, 0, 0, 0, 0, 0};
//static struct FlowMeter flowMeterSpargeIn = {FLOW_METER_GPIO_SPARGE_IN, 0, 0, FLOW_METER_CAL_LARGE, 0, 0, 0, 0, 0};
//static struct FlowMeter flowMeterSpargeOut = {FLOW_METER_GPIO_SPARGE_OUT, 0, 0, FLOW_METER_CAL_LARGE, 0, 0, 0, 0, 0};

int get_meter_flow_rate(struct FlowMeter *flowMeter);
int get_meter_flow_total(struct FlowMeter *flowMeter);
void reset_meter_flow_total(struct FlowMeter *flowMeter);

void button(struct Interrupts *intr);
void flowM(void);
void interrupts_config(void);
void getFlowRate(uint32_t timeDifference);
void updateFlowMeter(struct FlowMeter *flowMeter);
