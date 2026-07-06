#include <string.h>

#include "btstack_port_esp32.h"
#include "btstack_run_loop.h"
#include "btstack_util.h"
#include "controller/uni_gamepad.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uni.h"

#include "bluetooth/bluetooth.h"
#include "bluetooth/bluetooth_event.h"
#include "config/config.h"
#include "gamepad/gamepad.h"

#define TAG "bluetooth"

static const AppConfig* s_config;
static BluetoothGamepadCallback s_gamepad_callback;
static void* s_gamepad_callback_context;
static bool s_preferred_controller_enabled;
static bool s_preferred_controller_valid;
static TickType_t s_preferred_controller_deadline;
static bd_addr_t s_preferred_controller_addr;
static bool s_connected = false;

static void emit_event(const BluetoothEvent* event)
{
    if (s_gamepad_callback != NULL)
    {
        s_gamepad_callback(
            event,
            s_gamepad_callback_context);
    }
}

static void preferred_controller_init(const AppConfig* config)
{
	memset(s_preferred_controller_addr, 0, sizeof(s_preferred_controller_addr));
	s_preferred_controller_enabled = false;
	s_preferred_controller_valid = false;

	if (config == NULL || config->preferred_mac[0] == '\0') {
		return;
	}

	if (!sscanf_bd_addr(config->preferred_mac, s_preferred_controller_addr)) {
		ESP_LOGW(TAG, "MAC preferida invalida: %s", config->preferred_mac);
		return;
	}

	s_preferred_controller_enabled = true;
	s_preferred_controller_valid = true;
	s_preferred_controller_deadline = xTaskGetTickCount() + pdMS_TO_TICKS(config->preferred_controller_timeout_ms);

	ESP_LOGI(TAG, "Preferindo controle %s por %u ms", config->preferred_mac,
			 (unsigned)config->preferred_controller_timeout_ms);
}

static void platform_init(int argc, const char** argv)
{
	(void)argc;
	(void)argv;

	preferred_controller_init(s_config);
	ESP_LOGI(TAG, "Bluepad32 platform inicializada");
}

static void platform_on_init_complete(void)
{
	ESP_LOGI(TAG, "Bluetooth pronto, iniciando busca por controles");
	uni_gamepad_set_mappings_type(UNI_GAMEPAD_MAPPINGS_TYPE_XBOX);
	uni_bt_start_scanning_and_autoconnect_unsafe();
	uni_bt_allow_incoming_connections(true);
}

static uni_error_t platform_on_device_discovered(bd_addr_t addr, const char* name, uint16_t cod, uint8_t rssi)
{
	(void)name;
	(void)cod;
	(void)rssi;

	if (!s_preferred_controller_enabled || !s_preferred_controller_valid) {
		return UNI_ERROR_SUCCESS;
	}

	if (xTaskGetTickCount() >= s_preferred_controller_deadline) {
		s_preferred_controller_enabled = false;
		ESP_LOGI(TAG, "Janela da MAC preferida expirou, usando descoberta normal");
		return UNI_ERROR_SUCCESS;
	}

	if (bd_addr_cmp(addr, s_preferred_controller_addr) == 0) {
		ESP_LOGI(TAG, "MAC preferida encontrada: %s", bd_addr_to_str(addr));
		s_preferred_controller_enabled = false;
		return UNI_ERROR_SUCCESS;
	}

	ESP_LOGI(TAG, "Ignorando temporariamente %s enquanto aguarda %s", bd_addr_to_str(addr),
			 s_config->preferred_mac);
	return UNI_ERROR_IGNORE_DEVICE;
}

static void platform_on_device_connected(uni_hid_device_t* d)
{
	ESP_LOGI(TAG, "Controle conectado: %p", (void*)d);
	s_connected = true;
}

static void platform_on_device_disconnected(uni_hid_device_t* d)
{
	ESP_LOGI(TAG, "Controle desconectado: %p", (void*)d);
	s_connected = false;

	BluetoothEvent event = {
		.type = BLUETOOTH_EVENT_DISCONNECTED
	};

	emit_event(&event);
}

static uni_error_t platform_on_device_ready(uni_hid_device_t* d)
{
	(void)d;

	s_connected = true;

	BluetoothEvent event = {
			.type = BLUETOOTH_EVENT_CONNECTED,
	};

	emit_event(&event);

	ESP_LOGI(TAG, "Controle pronto: %p", (void*)d);
	return UNI_ERROR_SUCCESS;
}

static void platform_on_oob_event(uni_platform_oob_event_t event, void* data)
{
    (void)event;
    (void)data;
}

static void platform_on_controller_data(uni_hid_device_t* d, uni_controller_t* ctl)
{
	(void)d;

	if (ctl->klass != UNI_CONTROLLER_CLASS_GAMEPAD) {
		ESP_LOGW(TAG, "Nao é gamepad");
		return;
	}

	BluetoothEvent event = {0};

	event.type = BLUETOOTH_EVENT_COMMAND;

	gamepad_parse_controller(&ctl->gamepad, s_config, &event.data.command);

	emit_event(&event);
}

static const uni_property_t* platform_get_property(uni_property_idx_t idx)
{
    (void)idx;
    return NULL;
}

static struct uni_platform* get_my_platform(void)
{
	static struct uni_platform platform = {
			.name = "remote_car",
			.init = platform_init,
			.on_init_complete = platform_on_init_complete,
			.on_device_discovered = platform_on_device_discovered,
			.on_device_connected = platform_on_device_connected,
			.on_device_disconnected = platform_on_device_disconnected,
			.on_device_ready = platform_on_device_ready,
			.on_oob_event = platform_on_oob_event,
			.on_controller_data = platform_on_controller_data,
			.get_property = platform_get_property,
	};

	return &platform;
}

void bluetooth_init(const AppConfig* config, BluetoothGamepadCallback callback, void* context)
{
	s_config = config;
	s_gamepad_callback = callback;
	s_gamepad_callback_context = context;

	btstack_init();
	uni_platform_set_custom(get_my_platform());
	uni_init(0, NULL);
}

void bluetooth_run(void)
{
	btstack_run_loop_execute();
}

bool bluetooth_is_connected(void)
{
	return s_connected;
}
