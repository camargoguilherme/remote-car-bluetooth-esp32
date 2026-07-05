
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "config/config.h"

#define TAG "config"
#define CONFIG_NAMESPACE "rc_car"

static AppConfig s_config;

static void set_defaults(AppConfig* config)
{
	memset(config, 0, sizeof(*config));
	config->preferred_mac[0] = '\0';
	config->input_timeout_ms = 300;
	config->preferred_controller_timeout_ms = 10000;
	config->steering_deadzone = 10;
	config->throttle_deadzone = 10;
	config->invert_drive_motor = false;
	config->invert_steering_motor = false;
	config->invert_steering = false;
}

static esp_err_t load_string(nvs_handle_t handle, const char* key, char* out, size_t out_size)
{
	size_t required = out_size;
	esp_err_t err = nvs_get_str(handle, key, out, &required);
	if (err == ESP_ERR_NVS_NOT_FOUND) {
		out[0] = '\0';
		return ESP_OK;
	}
	if (err != ESP_OK) {
		return err;
	}
	out[out_size - 1] = '\0';
	return ESP_OK;
}

static esp_err_t save_string(nvs_handle_t handle, const char* key, const char* value)
{
	return nvs_set_str(handle, key, value != NULL ? value : "");
}

void config_restore_defaults(void)
{
	set_defaults(&s_config);
}

void config_init(void)
{
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);

	config_restore_defaults();
	config_load();
}

void config_load(void)
{
	nvs_handle_t handle;
	esp_err_t err = nvs_open(CONFIG_NAMESPACE, NVS_READONLY, &handle);
	if (err == ESP_ERR_NVS_NOT_FOUND) {
		config_restore_defaults();
		config_save();
		return;
	}

	if (err != ESP_OK) {
		ESP_LOGW(TAG, "Falha ao abrir NVS para leitura: %s", esp_err_to_name(err));
		return;
	}

	load_string(handle, "preferred_mac", s_config.preferred_mac, sizeof(s_config.preferred_mac));
	nvs_get_u32(handle, "input_timeout_ms", &s_config.input_timeout_ms);
	nvs_get_u32(handle, "preferred_controller_timeout_ms", &s_config.preferred_controller_timeout_ms);

	int32_t value = 0;
	if (nvs_get_i32(handle, "steering_deadzone", &value) == ESP_OK) {
		s_config.steering_deadzone = (int)value;
	}
	if (nvs_get_i32(handle, "throttle_deadzone", &value) == ESP_OK) {
		s_config.throttle_deadzone = (int)value;
	}

	uint8_t bool_value = 0;
	if (nvs_get_u8(handle, "invert_drive_motor", &bool_value) == ESP_OK) {
		s_config.invert_drive_motor = bool_value != 0;
	}
	if (nvs_get_u8(handle, "invert_steering_motor", &bool_value) == ESP_OK) {
		s_config.invert_steering_motor = bool_value != 0;
	}
	if (nvs_get_u8(handle, "invert_steering", &bool_value) == ESP_OK) {
		s_config.invert_steering = bool_value != 0;
	}

	nvs_close(handle);
}

void config_save(void)
{
	nvs_handle_t handle;
	esp_err_t err = nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &handle);
	if (err != ESP_OK) {
		ESP_LOGW(TAG, "Falha ao abrir NVS para escrita: %s", esp_err_to_name(err));
		return;
	}

	ESP_ERROR_CHECK(save_string(handle, "preferred_mac", s_config.preferred_mac));
	ESP_ERROR_CHECK(nvs_set_u32(handle, "input_timeout_ms", s_config.input_timeout_ms));
	ESP_ERROR_CHECK(nvs_set_u32(handle, "preferred_controller_timeout_ms", s_config.preferred_controller_timeout_ms));
	ESP_ERROR_CHECK(nvs_set_i32(handle, "steering_deadzone", s_config.steering_deadzone));
	ESP_ERROR_CHECK(nvs_set_i32(handle, "throttle_deadzone", s_config.throttle_deadzone));
	ESP_ERROR_CHECK(nvs_set_u8(handle, "invert_drive_motor", s_config.invert_drive_motor ? 1 : 0));
	ESP_ERROR_CHECK(nvs_set_u8(handle, "invert_steering_motor", s_config.invert_steering_motor ? 1 : 0));
	ESP_ERROR_CHECK(nvs_set_u8(handle, "invert_steering", s_config.invert_steering ? 1 : 0));
	ESP_ERROR_CHECK(nvs_commit(handle));
	nvs_close(handle);
}

const AppConfig* config_get(void)
{
	return &s_config;
}

void config_set(const AppConfig* config)
{
	if (config == NULL) {
		return;
	}

	s_config = *config;
	s_config.preferred_mac[sizeof(s_config.preferred_mac) - 1] = '\0';
}
