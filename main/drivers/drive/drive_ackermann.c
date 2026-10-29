#include "board/board.h"
#include "drivers/motors.h"
#include "drivers/servo.h"
#include "drivers/drive/drive.h"
#include "vehicle/vehicle.h"

void drive_ackermann_stop(void);

void drive_ackermann_init(void)
{
  servo_init();
}

void drive_ackermann_apply(const VehicleState* vehicle)
{
  if (vehicle == NULL)
  {
    drive_ackermann_stop();
    return;
  }

  motors_set_speed(BOARD_DRIVE_MOTOR, vehicle->motion.throttle);
  servo_set_steering(vehicle->motion.steering);
}

void drive_ackermann_stop(void)
{
  motors_stop();
  servo_set_steering(0);
}
