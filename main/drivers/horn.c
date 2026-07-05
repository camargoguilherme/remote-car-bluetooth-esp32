
#include "driver/gpio.h"

#include "board/board.h"
#include "drivers/horn.h"

#define TAG "horn"

void horn_init(void)
{
    gpio_config_t io = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << BOARD_PIN_HORN),
    };

    gpio_config(&io);

    gpio_set_level(BOARD_PIN_HORN, 0);
}

void horn_apply(const VehicleState* vehicle)
{
    if (vehicle == NULL)
        return;

    gpio_set_level(
        BOARD_PIN_HORN,
        vehicle->horn.enabled ? 1 : 0);
}
