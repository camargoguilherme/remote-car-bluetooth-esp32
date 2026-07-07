#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "app.h"
#include "main.h"

#define TAG "main"

void app_main(void)
{
    app_init();
    app_run();
}
