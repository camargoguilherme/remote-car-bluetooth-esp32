#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "board/board.h"
#include "drivers/lights.h"
#include "drivers/pwm.h"

#define TAG "lights"

static bool s_blink_state = false;
static TickType_t s_last_blink = 0;

static void update_blink(void)
{
    TickType_t now = xTaskGetTickCount();

    if ((now - s_last_blink) >= pdMS_TO_TICKS(500))
    {
        s_last_blink = now;
        s_blink_state = !s_blink_state;
    }
}

void lights_init(void)
{
    gpio_config_t io = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask =
            // Faróis 
            (1ULL << BOARD_PIN_HEADLIGHT) |
            
            // Lanternas
            (1ULL << BOARD_PIN_TAIL_LIGHT) |
            (1ULL << BOARD_PIN_BRAKE_LIGHT) |
            (1ULL << BOARD_PIN_REVERSE_LIGHT) |

            // Setas
            (1ULL << BOARD_PIN_LEFT_INDICATOR) |
            (1ULL << BOARD_PIN_RIGHT_INDICATOR),
    };

    gpio_config(&io);
    pwm_attach_channel(BOARD_CHANNEL_HEADLIGHT, BOARD_PIN_HEADLIGHT);
}

void lights_apply(const VehicleState* vehicle)
{
    update_blink();

    if (vehicle == NULL)
        return;

    // Faróis
    uint32_t duty = 0;

    switch (vehicle->lights.headlight)
    {
        case HEADLIGHT_OFF:
            duty = 0;
            break;

        case HEADLIGHT_LOW:
            duty = pwm_get_max_duty() * 30 / 100;
            break;

        case HEADLIGHT_MEDIUM:
            duty = pwm_get_max_duty() * 60 / 100;
            break;

        case HEADLIGHT_HIGH:
            duty = pwm_get_max_duty();
            break;
    }

    pwm_set_duty(BOARD_CHANNEL_HEADLIGHT, duty);
    
    // Lanternas
    gpio_set_level(BOARD_PIN_TAIL_LIGHT, vehicle->lights.tail_light);
    gpio_set_level(BOARD_PIN_BRAKE_LIGHT, vehicle->lights.brake_light);
    gpio_set_level(BOARD_PIN_REVERSE_LIGHT, vehicle->lights.reverse_light);

    // Setas
    bool left_indicator = (vehicle->lights.left_indicator || vehicle->lights.hazard) && s_blink_state;

    bool right_indicator = (vehicle->lights.right_indicator || vehicle->lights.hazard) && s_blink_state;

    gpio_set_level(BOARD_PIN_LEFT_INDICATOR, left_indicator);
    gpio_set_level(BOARD_PIN_RIGHT_INDICATOR, right_indicator);
}
