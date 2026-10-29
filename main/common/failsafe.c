#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "board/board.h"
#include "common/failsafe.h"
#include "config/config.h"
#include "app_config.h"

#define TAG "failsafe"

static volatile TickType_t s_last_input_tick;
static uint32_t s_input_timeout_ms;
static FailsafeStopCallback s_stop_callback;
static bool s_failsafe_active = false;

static void failsafe_task(void* arg)
{
	(void)arg;

	while (true) {
		TickType_t now = xTaskGetTickCount();
		TickType_t elapsed = now - s_last_input_tick;

		if (elapsed > pdMS_TO_TICKS(s_input_timeout_ms)) {
			if (!s_failsafe_active) {
				s_failsafe_active = true;
				if (s_stop_callback != NULL) {
					s_stop_callback();
				}
			}
		} else {
			if (s_failsafe_active) {
				s_failsafe_active = false;
			}
		}

		vTaskDelay(pdMS_TO_TICKS(BOARD_FAILSAFE_PERIOD_MS));
	}
}

esp_err_t failsafe_init(const AppConfig* config, FailsafeStopCallback stop_callback)
{
	s_last_input_tick = xTaskGetTickCount();
	s_input_timeout_ms = config != NULL ? config->input_timeout_ms : 300;
	s_stop_callback = stop_callback;
	s_failsafe_active = false; 

	BaseType_t ret = xTaskCreate(failsafe_task, "failsafe_task",
			BOARD_TASK_STACK_FAILSAFE,
			NULL,
			BOARD_TASK_PRIORITY_FAILSAFE,
			NULL);
    
    return (ret == pdPASS) ? ESP_OK : ESP_FAIL;
}

void failsafe_notify_input(void)
{
	s_last_input_tick = xTaskGetTickCount();
	s_failsafe_active = false;
}

void failsafe_set_enabled(bool enabled)
{
	s_failsafe_active = enabled;
}

bool failsafe_is_active(void)
{
    return s_failsafe_active;
}
