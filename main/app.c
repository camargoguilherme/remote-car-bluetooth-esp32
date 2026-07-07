#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "bluetooth/bluetooth.h"
#include "bluetooth/bluetooth_event.h"
#include "board/board.h"
#include "common/failsafe.h"
#include "config/config.h"
#include "gamepad/gamepad.h"
#include "vehicle/vehicle.h"
#include "app.h"
#include "app_config.h"


static void on_bluetooth_event(const BluetoothEvent* event, void* context)
{
  (void)context;

  switch (event->type)
  {
    case BLUETOOTH_EVENT_CONNECTED:
    case BLUETOOTH_EVENT_DISCONNECTED:
      vehicle_reset();
      break;

    case BLUETOOTH_EVENT_COMMAND:
      vehicle_apply_command(&event->data.command);
      failsafe_notify_input();
      break;
  }
}

void app_init(void)
{
  config_init();
  board_init();
  vehicle_init();
  gamepad_init();
  failsafe_init(config_get(), motors_stop);
  bluetooth_init(config_get(), on_bluetooth_event, NULL);
}

void app_run(void)
{
  bluetooth_run();
}
