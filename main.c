/**
 * Copyright (c) 2014 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 * This file contains the source code for a sample application to blink LEDs.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"

#define BUTTON_PIN      NRF_GPIO_PIN_MAP(1, 6)
#define LED_YELLOW      NRF_GPIO_PIN_MAP(0, 6)
#define LED_RED         NRF_GPIO_PIN_MAP(0, 8)
#define LED_GREEN       NRF_GPIO_PIN_MAP(1, 9)
#define LED_BLUE        NRF_GPIO_PIN_MAP(0, 12)


const uint32_t leds[4] = {LED_YELLOW, LED_RED, LED_GREEN, LED_BLUE};

void custom_gpio_init(void) {
    nrf_gpio_cfg_input(BUTTON_PIN, NRF_GPIO_PIN_PULLUP);
    for (int i = 0; i < 4; i++) {
        nrf_gpio_cfg_output(leds[i]);
        nrf_gpio_pin_write(leds[i], 1);
    }
}


void delay_with_button_pause(uint32_t ms) 
{
    for (uint32_t i = 0; i < ms; i++) {
        while (nrf_gpio_pin_read(BUTTON_PIN) != 0) 
        {

        }
        nrf_delay_ms(1);
    }
}


int main(void)
{
    custom_gpio_init();

    int sequence[4] = {6, 5, 7, 7}; 
    
    while (true)
    {
        for (int color_idx = 0; color_idx < 4; color_idx++) 
        {
            for (int blink = 0; blink < sequence[color_idx]; blink++) 
            {
                nrf_gpio_pin_write(leds[color_idx], 0);
                delay_with_button_pause(300); 
                nrf_gpio_pin_write(leds[color_idx], 1);
                delay_with_button_pause(300); 
            }
            delay_with_button_pause(1000); 
        }
        delay_with_button_pause(1500);
    }
}

/**
 *@}
 **/
