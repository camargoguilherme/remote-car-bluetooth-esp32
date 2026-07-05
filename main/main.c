#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "bluetooth/bluetooth.h"
#include "board/board.h"
#include "config/config.h"
#include "drivers/horn.h"
#include "drivers/lights.h"
#include "drivers/motors.h"
#include "gamepad/gamepad.h"
#include "vehicle/vehicle.h"
#include "failsafe.h"
#include "main.h"

#define DELAY(ms) vTaskDelay(pdMS_TO_TICKS(ms))
#define DELAY_1S DELAY(1000)

#define TAG "main"

static void on_gamepad_state_changed(const VehicleState* state, void* context)
{
    (void)context;

    motors_apply(state);
    lights_apply(state);
    horn_apply(state);

    if (state != NULL && state->status.connected) {
        failsafe_notify_input();
    }
}

void app_main(void)
{
    config_init();
    board_init();
    vehicle_init();
    gamepad_init();
    motors_init(config_get());
    horn_init();
    lights_init();
    failsafe_init(config_get(), motors_stop);
    bluetooth_init(config_get(), on_gamepad_state_changed, NULL);
    bluetooth_run();
}
