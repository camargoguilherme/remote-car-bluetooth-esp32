#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "board/board.h"
#include "common/util.h"
#include "drivers/lights.h"
#include "drivers/pwm.h"
#include "app_config.h"


static bool s_blink_state = false;
static TickType_t s_last_blink = 0;

static void update_blink(void)
{
    TickType_t now = xTaskGetTickCount();

    if ((now - s_last_blink) >= pdMS_TO_TICKS(BOARD_BLINK_INTERVAL_MS))
    {
        s_last_blink = now;
        s_blink_state = !s_blink_state;
    }
}

static uint32_t headlight_to_duty(HeadlightMode mode)
{
    switch (mode)
    {
        case HEADLIGHT_LOW:
            return util_percent_to_duty(BOARD_HEADLIGHT_LOW_PERCENT, pwm_get_max_duty());

        case HEADLIGHT_MEDIUM:
            return util_percent_to_duty(BOARD_HEADLIGHT_MEDIUM_PERCENT, pwm_get_max_duty());

        case HEADLIGHT_HIGH:
            return util_percent_to_duty(BOARD_HEADLIGHT_HIGH_PERCENT, pwm_get_max_duty());

        case HEADLIGHT_OFF:
        default:
            return 0;
    }
}

static void apply_headlight(const VehicleLightsState* lights)
{
    pwm_set_duty(
        BOARD_CHANNEL_HEADLIGHT,
        headlight_to_duty(lights->headlight));
}

static void apply_tail_lights(const VehicleLightsState* lights)
{
    gpio_set_level(BOARD_PIN_TAIL_LIGHT, lights->tail_light);
    gpio_set_level(BOARD_PIN_BRAKE_LIGHT, lights->brake_light);
    gpio_set_level(BOARD_PIN_REVERSE_LIGHT, lights->reverse_light);
}

static void apply_indicators(const VehicleLightsState* lights)
{
    bool left =
        (lights->left_indicator || lights->hazard) &&
        s_blink_state;

    bool right =
        (lights->right_indicator || lights->hazard) &&
        s_blink_state;

    gpio_set_level(BOARD_PIN_LEFT_INDICATOR, left);
    gpio_set_level(BOARD_PIN_RIGHT_INDICATOR, right);
}

void lights_init(void)
{
    gpio_config_t io =
    {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask =
            (1ULL << BOARD_PIN_HEADLIGHT) |
            (1ULL << BOARD_PIN_TAIL_LIGHT) |
            (1ULL << BOARD_PIN_BRAKE_LIGHT) |
            (1ULL << BOARD_PIN_REVERSE_LIGHT) |
            (1ULL << BOARD_PIN_LEFT_INDICATOR) |
            (1ULL << BOARD_PIN_RIGHT_INDICATOR),
    };

    gpio_config(&io);

    pwm_attach_channel(
        BOARD_CHANNEL_HEADLIGHT,
        BOARD_PIN_HEADLIGHT);
}

void lights_apply(const VehicleState* vehicle)
{
    if (vehicle == NULL)
        return;

    update_blink();

    apply_headlight(&vehicle->lights);
    apply_tail_lights(&vehicle->lights);
    apply_indicators(&vehicle->lights);
}
