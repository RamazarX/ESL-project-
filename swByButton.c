#include "swByButton.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "app_timer.h"
#include "nrf_log.h"

#define BTN_PIN             NRF_GPIO_PIN_MAP(1, 6)
#define DEBOUNCE_DELAY_MS   30
#define DOUBLE_CLICK_MAX_MS 500

APP_TIMER_DEF(m_debounce_timer);
APP_TIMER_DEF(m_double_click_timer);

static volatile bool is_active = false;
static volatile uint8_t click_count = 0;

static void debounce_timeout_handler(void * p_context)
{
    if (nrf_gpio_pin_read(BTN_PIN) == 0)
    {
        click_count++;
        if (click_count == 1)
        {
            app_timer_start(m_double_click_timer, APP_TIMER_TICKS(DOUBLE_CLICK_MAX_MS), NULL);
        }
        else if (click_count == 2)
        {
            app_timer_stop(m_double_click_timer);
            click_count = 0;
            is_active = !is_active;

            if (is_active)
            {
                NRF_LOG_INFO("Work resumed: blinking started.");
            }
            else
            {
                NRF_LOG_INFO("Work paused: blinking frozen.");
            }
        }
    }
}

static void double_click_timeout_handler(void * p_context)
{
    click_count = 0;
}

static void on_button_pressed(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    app_timer_stop(m_debounce_timer);
    app_timer_start(m_debounce_timer, APP_TIMER_TICKS(DEBOUNCE_DELAY_MS), NULL);
}

void button_init(void)
{
    app_timer_init();
    app_timer_create(&m_debounce_timer, APP_TIMER_MODE_SINGLE_SHOT, debounce_timeout_handler);
    app_timer_create(&m_double_click_timer, APP_TIMER_MODE_SINGLE_SHOT, double_click_timeout_handler);
    nrfx_gpiote_in_config_t pin_settings = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    pin_settings.pull = NRF_GPIO_PIN_PULLUP;
    if (!nrfx_gpiote_is_init())
    {
        nrfx_gpiote_init();
    }
    nrfx_gpiote_in_init(BTN_PIN, &pin_settings, on_button_pressed);
    nrfx_gpiote_in_event_enable(BTN_PIN, true);
}

bool button_is_playing(void)
{
    return is_active;
}