#include <stdbool.h>
#include <stdint.h>

#include "driver/ledc.h"
#include "esp_err.h"

#include "app_config.h"
#include "board/board.h"
#include "common/util.h"
#include "drivers/pwm.h"
#include "drivers/servo.h"

#define SERVO_PERIOD_US (1000000U / BOARD_SERVO_FREQUENCY_HZ)
#define SERVO_DUTY_SCALE (1U << 16)

static bool s_initialized;
static int s_last_steering = 101;

void servo_set_steering(int steering_percent)
{
    if (!s_initialized)
        return;

    steering_percent = util_clamp_percent(steering_percent);
    if (steering_percent == s_last_steering)
        return;

    s_last_steering = steering_percent;

    uint32_t pulse_range = BOARD_SERVO_MAX_PULSE_US - BOARD_SERVO_MIN_PULSE_US;
    uint32_t pulse_us = BOARD_SERVO_MIN_PULSE_US +
        ((uint32_t)(steering_percent + 100) * pulse_range) / 200U;
    uint32_t duty = (pulse_us * SERVO_DUTY_SCALE) / SERVO_PERIOD_US;

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, BOARD_CHANNEL_SERVO, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, BOARD_CHANNEL_SERVO));
}

void servo_init(void)
{
    if (s_initialized)
        return;

    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = BOARD_SERVO_RESOLUTION,
        .timer_num = BOARD_SERVO_TIMER,
        .freq_hz = BOARD_SERVO_FREQUENCY_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    pwm_attach_channel_to_timer(BOARD_CHANNEL_SERVO, BOARD_PIN_SERVO, BOARD_SERVO_TIMER);

    s_initialized = true;
    servo_set_steering(0);
}
