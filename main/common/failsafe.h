#pragma once

#include <stdint.h>

#include "config/config.h"
#include "esp_err.h"

typedef void (*FailsafeStopCallback)(void);

esp_err_t failsafe_init(const AppConfig* config, FailsafeStopCallback stop_callback);
void failsafe_notify_input(void);
void failsafe_set_enabled(bool enabled);
bool failsafe_is_active(void);
