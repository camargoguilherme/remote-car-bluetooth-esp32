#pragma once

#include "bluetooth_event.h"
#include "config/config.h"
#include "gamepad/gamepad.h"

typedef void (*BluetoothGamepadCallback)(const BluetoothEvent *event, void* context);

void bluetooth_init(const AppConfig* config, BluetoothGamepadCallback callback, void* context);
void bluetooth_run(void);
bool bluetooth_is_connected(void);
