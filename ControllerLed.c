#include "ControllerLed.h"
#include "nrf_gpio.h"
#include "nrfx_pwm.h"

#define PIN_LED1_GREEN   NRF_GPIO_PIN_MAP(0, 6)
#define PIN_LED2_RED     NRF_GPIO_PIN_MAP(0, 8)
#define PIN_LED2_GREEN   NRF_GPIO_PIN_MAP(1, 9)
#define PIN_LED2_BLUE    NRF_GPIO_PIN_MAP(0, 12)
#define PWM_TOP_VALUE    1000

static nrfx_pwm_t m_pwm0 = NRFX_PWM_INSTANCE(0);
static nrf_pwm_values_individual_t m_seq_values = {0, 0, 0, 0};
static nrf_pwm_sequence_t const m_seq = {
    .values.p_individual = &m_seq_values,
    .length= NRF_PWM_VALUES_LENGTH(m_seq_values),
    .repeats= 0,
    .end_delay= 0
};

static const int blinks_target[4] = {6, 5, 7, 7};
static int current_led   = 0;
static int current_blink = 0;
static int fee= 0;
static int step= 20;

void leds_init(void) 
{
    nrf_gpio_cfg_output(PIN_LED1_GREEN);
    nrf_gpio_cfg_output(PIN_LED2_RED);
    nrf_gpio_cfg_output(PIN_LED2_GREEN);
    nrf_gpio_cfg_output(PIN_LED2_BLUE);

    nrf_gpio_pin_set(PIN_LED1_GREEN);
    nrf_gpio_pin_set(PIN_LED2_RED);
    nrf_gpio_pin_set(PIN_LED2_GREEN);
    nrf_gpio_pin_set(PIN_LED2_BLUE);

    nrfx_pwm_config_t pwm_cfg = {
        .output_pins = { PIN_LED1_GREEN | NRFX_PWM_PIN_INVERTED, PIN_LED2_RED   | NRFX_PWM_PIN_INVERTED, PIN_LED2_GREEN | NRFX_PWM_PIN_INVERTED, PIN_LED2_BLUE  | NRFX_PWM_PIN_INVERTED },
        .irq_priority = 7, .base_clock= NRF_PWM_CLK_1MHz, .count_mode= NRF_PWM_MODE_UP, .top_value = PWM_TOP_VALUE, .load_mode= NRF_PWM_LOAD_INDIVIDUAL, .step_mode= NRF_PWM_STEP_AUTO
    };

    m_seq_values.channel_0 = 0;
    m_seq_values.channel_1 = 0;
    m_seq_values.channel_2 = 0;
    m_seq_values.channel_3 = 0;

    nrfx_pwm_init(&m_pwm0, &pwm_cfg, NULL);
    nrfx_pwm_simple_playback(&m_pwm0, &m_seq, 1, NRFX_PWM_FLAG_LOOP);
}

void leds_process(bool is_playing) 
{
    if (is_playing) 
    {
        while (blinks_target[current_led] == 0) 
        {
            current_led = (current_led + 1) % 4;
            current_blink = 0;
        }

        fee += step;

        if (fee >= PWM_TOP_VALUE) 
        {
            fee = PWM_TOP_VALUE;
            step = -20;
        } else if (fee <= 0) 
        {
            fee = 0;
            step = 20;
            current_blink++;

            if (current_blink >= blinks_target[current_led]) {
                current_blink = 0;
                current_led = (current_led + 1) % 4;

                while (blinks_target[current_led] == 0) {
                    current_led = (current_led + 1) % 4;
                }
            }
        }

        uint16_t ch0 = 0, ch1 = 0, ch2 = 0, ch3 = 0;

        if (current_led == 0)      ch0 = (uint16_t)fee;
        else if (current_led == 1) ch1 = (uint16_t)fee;
        else if (current_led == 2) ch2 = (uint16_t)fee;
        else if (current_led == 3) ch3 = (uint16_t)fee;

        m_seq_values.channel_0 = ch0;
        m_seq_values.channel_1 = ch1;
        m_seq_values.channel_2 = ch2;
        m_seq_values.channel_3 = ch3;
    }
}