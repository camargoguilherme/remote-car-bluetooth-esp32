#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t previous_buttons;
    uint8_t previous_misc_buttons;

} ButtonState;

void button_state_init(ButtonState* state);
bool button_pressed(ButtonState* state, uint32_t current_buttons, uint32_t button_mask);
bool misc_button_pressed(ButtonState* state, uint8_t current_buttons, uint8_t button_mask);
void button_state_update(ButtonState* state, uint32_t current_buttons, uint8_t current_misc_buttons);
