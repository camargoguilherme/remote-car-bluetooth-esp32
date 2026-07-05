#include "driver/gpio.h"

#include "board/board.h"
#include "drivers/lights.h"

#define TAG "lights"

void lights_init(void)
{
    gpio_config_t io = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask =
            // Faróis 
            (1ULL << BOARD_PIN_HEADLIGHT_LOW) |
            (1ULL << BOARD_PIN_HEADLIGHT_MEDIUM) |
            (1ULL << BOARD_PIN_HEADLIGHT_HIGH) |
            
            // Lanternas
            (1ULL << BOARD_PIN_TAIL_LIGHT) |
            (1ULL << BOARD_PIN_BRAKE_LIGHT) |
            (1ULL << BOARD_PIN_REVERSE_LIGHT) |

            // Setas
            (1ULL << BOARD_PIN_LEFT_INDICATOR) |
            (1ULL << BOARD_PIN_RIGHT_INDICATOR),
    };

    gpio_config(&io);
}

void lights_apply(const VehicleState* vehicle)
{
    if (vehicle == NULL)
        return;

    // Faróis
    gpio_set_level(BOARD_PIN_HEADLIGHT_LOW, vehicle->lights.headlight == HEADLIGHT_LOW);
    gpio_set_level(BOARD_PIN_HEADLIGHT_MEDIUM, vehicle->lights.headlight == HEADLIGHT_MEDIUM);
    gpio_set_level(BOARD_PIN_HEADLIGHT_HIGH, vehicle->lights.headlight == HEADLIGHT_HIGH);

    // Lanternas
    gpio_set_level(BOARD_PIN_TAIL_LIGHT, vehicle->lights.tail_light);
    gpio_set_level(BOARD_PIN_BRAKE_LIGHT, vehicle->lights.brake_light);
    gpio_set_level(BOARD_PIN_REVERSE_LIGHT, vehicle->lights.reverse_light);

    // Setas
    gpio_set_level(BOARD_PIN_LEFT_INDICATOR, vehicle->lights.left_indicator);
    gpio_set_level(BOARD_PIN_RIGHT_INDICATOR, vehicle->lights.right_indicator);
}
