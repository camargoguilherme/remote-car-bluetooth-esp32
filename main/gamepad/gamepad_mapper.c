#include <stdio.h>

#include "esp_log.h"

#include "common/util.h"
#include "config/config.h"
#include "gamepad/gamepad_button_state.h"
#include "gamepad/gamepad_mapper.h"
#include "vehicle/vehicle_command.h"

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

    steering = util_axis_to_percent(controller->axis_x);

    int accel = util_axis_to_percent(controller->throttle);
    int brake = util_axis_to_percent(controller->brake);

    throttle = accel - brake;

    if (config != NULL) {
        steering = util_apply_deadzone(steering, config->steering_deadzone);
        throttle = util_apply_deadzone(throttle, config->throttle_deadzone);
        if (config->invert_steering)
            steering = -steering;
    }

    command->steering = steering;
    command->throttle = throttle;
    command->horn_pressed =  (controller->buttons & BUTTON_A) != 0;
    command->toggle_hazard = button_pressed(&s_button_state, controller->buttons, BUTTON_B);
    command->toggle_left_indicator =  button_pressed(&s_button_state, controller->buttons, BUTTON_SHOULDER_L);
    command->toggle_right_indicator = button_pressed(&s_button_state, controller->buttons, BUTTON_SHOULDER_R);
    command->headlight_up = dpad_pressed(&s_button_state, controller->dpad, DPAD_UP);
    command->headlight_down = dpad_pressed(&s_button_state, controller->dpad, DPAD_DOWN);

    button_state_update(&s_button_state, controller->buttons, controller->dpad, controller->misc_buttons);
}
