#include <stdbool.h>

#include "esp_check.h"

#include "drivers/pwm.h"

#define TAG "pwm"

#define PWM_FREQUENCY_HZ 20000
#define PWM_RESOLUTION LEDC_TIMER_10_BIT
#define PWM_TIMER LEDC_TIMER_0

static bool s_initialized;

static uint32_t pwm_max_duty(void)
{
    return (1U << PWM_RESOLUTION) - 1U;
}

void pwm_init(void)
{
    if (s_initialized) {
        return;
    }

    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = PWM_RESOLUTION,
        .timer_num = PWM_TIMER,
        .freq_hz = PWM_FREQUENCY_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));
    s_initialized = true;
}

void pwm_attach_channel(ledc_channel_t channel, gpio_num_t gpio)
{
    ledc_channel_config_t channel_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel,
        .timer_sel = PWM_TIMER,
        .gpio_num = gpio,
        .duty = 0,
        .hpoint = 0,
    };

    ESP_ERROR_CHECK(ledc_channel_config(&channel_config));
}

void pwm_set_duty(ledc_channel_t channel, uint32_t duty)
{
    if (duty > pwm_max_duty()) {
        duty = pwm_max_duty();
    }

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, channel));
}

uint32_t pwm_get_max_duty(void)
{
    return pwm_max_duty();
}
