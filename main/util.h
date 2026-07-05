#pragma once

int util_clamp_int(int value, int min_value, int max_value);
int util_clamp_percent(int value);
int util_apply_deadzone(int value, int deadzone);
int util_axis_to_percent(int axis_value);
