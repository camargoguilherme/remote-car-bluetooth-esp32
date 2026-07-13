#include <stdio.h>

#include "board/board.h"
#include "common/util.h"
#include "drivers/motors.h"
#include "drivers/drive/drive.h"
#include "vehicle/vehicle.h"


void drive_differential_init(void)
{
    // Nada para inicializar por enquanto.
}

void drive_differential_stop(void)
{
    motors_stop();
}

void drive_differential_apply(const VehicleState* vehicle)
{
    if (vehicle == NULL)
    {
        motors_stop();
        return;
    }

    int throttle = vehicle->motion.throttle;
    int steering = vehicle->motion.steering;

    /*
     * Mistura diferencial:
     *
     * steering > 0  -> direita
     * steering < 0  -> esquerda
     *
     * Motor 1 = lado esquerdo
     * Motor 2 = lado direito
     */
    int left_speed  = throttle + steering;
    int right_speed = throttle - steering;

    left_speed  = util_clamp_percent(left_speed);
    right_speed = util_clamp_percent(right_speed);

    motors_set_speed(BOARD_DRIVE_MOTOR, left_speed);
    motors_set_speed(BOARD_STEERING_MOTOR, right_speed);
}
