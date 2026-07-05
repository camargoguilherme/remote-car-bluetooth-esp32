#include <string.h>

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gamepad/gamepad_logger.h"

#define TAG "gamepad"

void gamepad_log(const uni_gamepad_t* controller)
{
  // Loga apenas quando houver mudança
	static uni_gamepad_t previous = {0};
	static TickType_t last_log = 0;

	TickType_t now = xTaskGetTickCount();

	if ((now - last_log) >= pdMS_TO_TICKS(1000) &&
			memcmp(&previous, controller, sizeof(*controller)) != 0) {

			previous = *controller;
			last_log = now;        
    
      ESP_LOGI(TAG, "========== GAMEPAD ==========");
      ESP_LOGI(TAG, "Buttons : 0x%08X", controller->buttons);
      ESP_LOGI(TAG, "DPad    : 0x%02X", controller->dpad);

      ESP_LOGI(TAG,
              "LX=%4ld  LY=%4ld  RX=%4ld  RY=%4ld",
              controller->axis_x,
              controller->axis_y,
              controller->axis_rx,
              controller->axis_ry);

      ESP_LOGI(TAG,
              "Brake=%4ld  Throttle=%4ld",
              controller->brake,
              controller->throttle);

      ESP_LOGI(TAG, "Misc: 0x%02X", controller->misc_buttons);

      ESP_LOGI(TAG, "=============================");

      // Log bruto recebido da Bluepad32
      ESP_LOGI(TAG,
        "Buttons=0x%08X DPad=0x%02X | "
        "LX=%4ld LY=%4ld RX=%4ld RY=%4ld | "
        "Brake=%4ld Throttle=%4ld",
        controller->buttons,
        controller->dpad,
        controller->axis_x,
        controller->axis_y,
        controller->axis_rx,
        controller->axis_ry,
        controller->brake,
        controller->throttle);

      ESP_LOGI(TAG, "=============================");

      if (controller->dpad & DPAD_UP)                  		ESP_LOGI(TAG, "DPAD_UP");
      if (controller->dpad & DPAD_DOWN)                		ESP_LOGI(TAG, "DPAD_DOWN");
      if (controller->dpad & DPAD_LEFT)                		ESP_LOGI(TAG, "DPAD_LEFT");
      if (controller->dpad & DPAD_RIGHT)               		ESP_LOGI(TAG, "DPAD_RIGHT");

      if (controller->buttons & BUTTON_A)              		ESP_LOGI(TAG, "BUTTON_A");
      if (controller->buttons & BUTTON_B)              		ESP_LOGI(TAG, "BUTTON_B");
      if (controller->buttons & BUTTON_X)              		ESP_LOGI(TAG, "BUTTON_X");
      if (controller->buttons & BUTTON_Y)              		ESP_LOGI(TAG, "BUTTON_Y");

      if (controller->buttons & BUTTON_SHOULDER_L)     		ESP_LOGI(TAG, "L1");
      if (controller->buttons & BUTTON_SHOULDER_R)     		ESP_LOGI(TAG, "R1");

      if (controller->buttons & BUTTON_TRIGGER_L)      		ESP_LOGI(TAG, "L2");
      if (controller->buttons & BUTTON_TRIGGER_R)      		ESP_LOGI(TAG, "R2");

      if (controller->buttons & BUTTON_THUMB_L)        		ESP_LOGI(TAG, "L3");
      if (controller->buttons & BUTTON_THUMB_R)        		ESP_LOGI(TAG, "R3");

      if (controller->misc_buttons & MISC_BUTTON_SELECT) 	    ESP_LOGI(TAG, "SELECT");
      if (controller->misc_buttons & MISC_BUTTON_START)  	    ESP_LOGI(TAG, "START");
      if (controller->misc_buttons & MISC_BUTTON_SYSTEM) 	    ESP_LOGI(TAG, "HOME");

    }
}
