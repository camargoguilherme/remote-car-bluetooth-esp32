#pragma once

#include <stdbool.h>

typedef struct
{
    // Movimento
    int steering;
    int throttle;

    // Iluminação
    bool toggle_left_indicator;
    bool toggle_right_indicator;
    bool toggle_hazard;
    bool toggle_headlight;

    // Som
    bool horn_pressed;

} VehicleCommand;

void vehicle_command_reset(VehicleCommand *cmd);
