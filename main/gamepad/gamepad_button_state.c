#include <stdio.h>

#include "gamepad/gamepad_button_state.h"

void button_state_init(ButtonState* state)
{
    if (state == NULL)
        return;

    state->previous_buttons = 0;
    state->previous_dpad = 0;
    state->previous_misc_buttons = 0;
}

static bool edge_pressed(uint32_t previous, uint32_t current, uint32_t mask)
{
    return ((previous & mask) == 0) && ((current & mask) != 0);
}

bool button_pressed(ButtonState* state, uint32_t current_buttons, uint32_t button_mask)
{
    if (state == NULL)
        return false;

    return edge_pressed(state->previous_buttons, current_buttons, button_mask);
}

bool misc_button_pressed(ButtonState* state,uint8_t  current_misc_buttons, uint8_t button_mask)
{
    if (state == NULL)
        return false;

    return edge_pressed(state->previous_misc_buttons, current_misc_buttons, button_mask);
}

bool dpad_pressed(ButtonState* state, uint8_t current_dpad, uint8_t dpad_mask)
{
    if (state == NULL)
        return false;

    return edge_pressed(state->previous_dpad, current_dpad, dpad_mask);
}

void button_state_update(ButtonState* state, uint32_t current_buttons, uint8_t current_dpad, uint8_t current_misc_buttons)
{
    if (state == NULL)
        return;

    state->previous_buttons = current_buttons;
    state->previous_dpad = current_dpad;
    state->previous_misc_buttons = current_misc_buttons;
}
