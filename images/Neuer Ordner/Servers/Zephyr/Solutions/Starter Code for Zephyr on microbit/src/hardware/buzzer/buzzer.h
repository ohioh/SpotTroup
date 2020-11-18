#define BUZZER_PIN EXT_P6_GPIO_PIN
#define PERIOD_MIN 50
#define PERIOD_MAX 3900
#define PERIOD_INIT 1500
#define BEEP_DURATION 500
#define US_TO_HZ(_us) (USEC_PER_SEC / (_us))

void buzzer_init();
void buzz_control(struct k_timer *timer_id);
void buzz(u8_t times);
