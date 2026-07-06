#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "app.h"
#include "main.h"

#define DELAY(ms) vTaskDelay(pdMS_TO_TICKS(ms))
#define DELAY_1S DELAY(1000)

#define TAG "main"

void app_main(void)
{
    app_init();
    app_run();
}
