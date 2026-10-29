#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "app.h"
#include "main.h"

#define TAG "main"

void app_main(void)
{
    ESP_ERROR_CHECK(app_init());
    app_run();
}
