#include <string.h>
#include <inttypes.h>

#include "controller/uni_gamepad.h"
#include "esp_log.h"

#include "config/config.h"
#include "gamepad/gamepad_logger.h"
#include "gamepad/gamepad_mapper.h"
#include "gamepad/gamepad.h"
#include "vehicle/vehicle.h"
#include "vehicle/vehicle_command.h"
#include "util.h"

#define TAG "gamepad"

void gamepad_init(void)
{
	ESP_LOGI(TAG, "Inicializando gamepad");
}

void gamepad_parse_controller(const uni_gamepad_t* controller, const AppConfig* config)
{
	VehicleCommand command;

	vehicle_command_reset(&command);

	gamepad_log(controller);	

	gamepad_map_controller(controller, config, &command);

	vehicle_apply_command(&command);
}
