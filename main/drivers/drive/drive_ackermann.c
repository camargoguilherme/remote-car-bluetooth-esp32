#include <stdio.h>

#include "board/board.h"
#include "drivers/motors.h"
#include "drivers/drive/drive.h"
#include "vehicle/vehicle.h"

void drive_ackermann_init(void)
{
}

void drive_ackermann_apply(const VehicleState* vehicle)
{
  if (vehicle == NULL)
  {
    motors_stop();
    return;
  }

  motors_set_speed(BOARD_LEFT_MOTOR, vehicle->motion.throttle);
  motors_set_speed(BOARD_RIGHT_MOTOR, vehicle->motion.steering);
}

void drive_ackermann_stop(void)
{
  motors_stop();
}
