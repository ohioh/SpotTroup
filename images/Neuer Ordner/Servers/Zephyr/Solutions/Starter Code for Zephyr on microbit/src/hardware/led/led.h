#define PORT SW0_GPIO_CONTROLLER
#define LED0 EXT_P3_GPIO_PIN
#define LED1 EXT_P4_GPIO_PIN
#define LED2 EXT_P5_GPIO_PIN
#define LED_SLEEP_TIME 500

void led_init();
void flash_control(struct k_timer *timer_id);
void flash(bool led0, bool led1, bool led2, u8_t times);
void state_control(struct k_timer *timer_id);
void set_led_states(bool led0, bool led1, bool led2);
void set_led_state(int led, bool state);
