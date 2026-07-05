#include <stdio.h>

#include "esp_log.h"

#include "gamepad/gamepad_button_state.h"
#include "config/config.h"
#include "gamepad/gamepad_mapper.h"
#include "vehicle/vehicle_command.h"
#include "util.h"

static ButtonState s_button_state;
static bool s_initialized = false;

#define TAG "gamepad/gamepad_mapper"

void gamepad_map_controller(const uni_gamepad_t* controller, const AppConfig* config, VehicleCommand* command)
{
    if (!s_initialized)
    {
        button_state_init(&s_button_state);
        s_initialized = true;
    }

    int steering = 0;
    int throttle = 0;

    if (controller == NULL || command == NULL)
        return;

    if (controller->dpad != 0) {

        if (controller->dpad & DPAD_UP)
            throttle = 100;

        if (controller->dpad & DPAD_DOWN)
            throttle = -100;

        if (controller->dpad & DPAD_LEFT)
            steering = -70;

        if (controller->dpad & DPAD_RIGHT)
            steering = 70;
    }
    else {

        throttle = -util_axis_to_percent(controller->axis_y);
        steering = util_axis_to_percent(controller->axis_x);
    }

    if (config != NULL) {

        steering = util_apply_deadzone(
            steering,
            config->steering_deadzone);

        throttle = util_apply_deadzone(
            throttle,
            config->throttle_deadzone);

        if (config->invert_steering)
            steering = -steering;
    }

    command->steering = steering;
    command->throttle = throttle;
    command->horn_pressed =  (controller->buttons & BUTTON_A) != 0;
    command->toggle_headlight = button_pressed(&s_button_state, controller->buttons, BUTTON_Y);
    command->toggle_left_indicator =  button_pressed(&s_button_state, controller->buttons, BUTTON_SHOULDER_L);
    command->toggle_right_indicator = button_pressed(&s_button_state, controller->buttons, BUTTON_SHOULDER_R);
    command->toggle_hazard = button_pressed(&s_button_state, controller->buttons, MISC_BUTTON_SELECT);

    button_state_update(&s_button_state, controller->buttons, controller->misc_buttons);

    if (command->toggle_headlight)
    {
        ESP_LOGI(TAG, "Toggle Headlight");
    }
}
