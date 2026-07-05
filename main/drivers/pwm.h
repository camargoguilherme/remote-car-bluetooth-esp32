#pragma once

#include <stdint.h>

#include "driver/gpio.h"
#include "driver/ledc.h"

void pwm_init(void);
void pwm_attach_channel(ledc_channel_t channel, gpio_num_t gpio);
void pwm_set_duty(ledc_channel_t channel, uint32_t duty);
uint32_t pwm_get_max_duty(void);
