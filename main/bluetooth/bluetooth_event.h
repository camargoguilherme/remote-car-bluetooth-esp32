#pragma once

#include "vehicle/vehicle_command.h"

typedef enum
{
    BLUETOOTH_EVENT_CONNECTED,
    BLUETOOTH_EVENT_DISCONNECTED,
    BLUETOOTH_EVENT_COMMAND
} BluetoothEventType;

typedef struct
{
    BluetoothEventType type;
    union
    {
        VehicleCommand command;
    } data;
} BluetoothEvent;
