#pragma once

#include <stdint.h>

#include "config/config.h"

typedef void (*FailsafeStopCallback)(void);

void failsafe_init(const AppConfig* config, FailsafeStopCallback stop_callback);
void failsafe_notify_input(void);
