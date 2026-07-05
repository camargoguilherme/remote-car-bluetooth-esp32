#pragma once

#include <stdbool.h>

#include "vehicle/vehicle_command.h"

typedef enum
{
    HEADLIGHT_OFF = 0,
    HEADLIGHT_LOW,
    HEADLIGHT_MEDIUM,
    HEADLIGHT_HIGH
} HeadlightMode;

typedef struct
{
    int steering;
    int throttle;

} VehicleMotionState;

typedef struct
{
    /* Dianteira */
    HeadlightMode headlight;

    /* Setas */
    bool left_indicator;
    bool right_indicator;
    bool hazard;

    /* Traseira */
    bool tail_light;
    bool brake_light;
    bool reverse_light;

} VehicleLightsState;

typedef struct
{
    bool enabled;

} VehicleHornState;

typedef struct
{
    bool connected;
    bool failsafe;
    bool emergency_stop;

} VehicleStatus;

typedef struct
{
    VehicleStatus status;

    VehicleMotionState motion;
    VehicleLightsState lights;
    VehicleHornState horn;

} VehicleState;

void vehicle_init(void);
VehicleState* vehicle_get(void);
void apply_motion(const VehicleCommand* command);
void apply_lights(const VehicleCommand* command);
void apply_horn(const VehicleCommand* command);
void vehicle_apply_command(const VehicleCommand* command);
void vehicle_reset(void);
