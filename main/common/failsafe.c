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

static void failsafe_task(void* arg)
{
	(void)arg;

	while (true) {
		TickType_t now = xTaskGetTickCount();
		TickType_t elapsed = now - s_last_input_tick;

		if (elapsed > pdMS_TO_TICKS(s_input_timeout_ms)) {
			if (s_stop_callback != NULL) {
				s_stop_callback();
			}
		}

		vTaskDelay(pdMS_TO_TICKS(BOARD_FAILSAFE_PERIOD_MS));
	}
}

void failsafe_init(const AppConfig* config, FailsafeStopCallback stop_callback)
{
	s_last_input_tick = xTaskGetTickCount();
	s_input_timeout_ms = config != NULL ? config->input_timeout_ms : 300;
	s_stop_callback = stop_callback;

	xTaskCreate(failsafe_task, "failsafe_task", 
			BOARD_TASK_STACK_FAILSAFE, 
			NULL, 
			BOARD_TASK_PRIORITY_FAILSAFE, 
			NULL);
}

void failsafe_notify_input(void)
{
	s_last_input_tick = xTaskGetTickCount();
}
