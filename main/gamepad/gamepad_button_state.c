#include <stdio.h>

#include "gamepad/gamepad_button_state.h"

void button_state_init(ButtonState* state)
{
    if (state == NULL)
        return;

    state->previous_buttons = 0;
    state->previous_misc_buttons = 0;
}

bool button_pressed(ButtonState* state, uint32_t current_buttons, uint32_t button_mask)
{
    if (state == NULL)
        return false;

    bool previous = (state->previous_buttons & button_mask) != 0;
    bool current = (current_buttons & button_mask) != 0;

    return (!previous && current);
}

bool misc_button_pressed(ButtonState* state,uint8_t current_buttons, uint8_t button_mask)
{
    if (state == NULL)
        return false;

    bool previous = (state->previous_misc_buttons & button_mask) != 0;
    bool current = (current_buttons & button_mask) != 0;

    return (!previous && current);
}

void button_state_update(ButtonState* state, uint32_t current_buttons, uint8_t current_misc_buttons)
{
    if (state == NULL)
        return;

    state->previous_buttons = current_buttons;
    state->previous_misc_buttons = current_misc_buttons;
}
