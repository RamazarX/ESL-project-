#ifndef CONTROLLER_LED_H
#define CONTROLLER_LED_H
#include <stdbool.h>
void leds_init(void);
void leds_process(bool is_playing);
#endif