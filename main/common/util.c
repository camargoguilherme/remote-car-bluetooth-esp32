#include "common/util.h"

#include <stdlib.h>

int util_clamp_int(int value, int min_value, int max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

int util_clamp_percent(int value)
{
    return util_clamp_int(value, -100, 100);
}

uint32_t util_percent_to_duty(int percent, uint32_t max_duty)
{
    percent = abs(util_clamp_percent(percent));

    return ((uint32_t)percent * max_duty) / 100;
}

int util_apply_deadzone(int value, int deadzone)
{
    if (deadzone < 0) {
        deadzone = -deadzone;
    }
    if (abs(value) < deadzone) {
        return 0;
    }
    return value;
}

int util_axis_to_percent(int axis_value)
{
    int percent = (axis_value * 100) / 512;
    return util_clamp_percent(percent);
}
