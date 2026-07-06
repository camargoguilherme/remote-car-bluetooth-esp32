#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "config/config.h"
#include "failsafe.h"

#define TAG "failsafe"

#define FAILSAFE_PERIOD_MS 50

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

		vTaskDelay(pdMS_TO_TICKS(FAILSAFE_PERIOD_MS));
	}
}

void failsafe_init(const AppConfig* config, FailsafeStopCallback stop_callback)
{
	s_last_input_tick = xTaskGetTickCount();
	s_input_timeout_ms = config != NULL ? config->input_timeout_ms : 300;
	s_stop_callback = stop_callback;

	xTaskCreate(failsafe_task, "failsafe_task", 3072, NULL, 5, NULL);
}

void failsafe_notify_input(void)
{
	s_last_input_tick = xTaskGetTickCount();
}
