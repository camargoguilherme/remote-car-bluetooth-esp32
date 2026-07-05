#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	char preferred_mac[18];
	uint32_t input_timeout_ms;
	uint32_t preferred_controller_timeout_ms;
	int steering_deadzone;
	int throttle_deadzone;
	bool invert_drive_motor;
	bool invert_steering_motor;
	bool invert_steering;
} AppConfig;

void config_init(void);
void config_load(void);
void config_save(void);
void config_restore_defaults(void);
const AppConfig* config_get(void);
void config_set(const AppConfig* config);
