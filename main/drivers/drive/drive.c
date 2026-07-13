#include "board/board.h"
#include "drivers/drive/drive.h"
#include "vehicle/vehicle.h"

/* Ackermann */
void drive_ackermann_init(void);
void drive_ackermann_apply(const VehicleState* vehicle);
void drive_ackermann_stop(void);

/* Differential */
void drive_differential_init(void);
void drive_differential_apply(const VehicleState* vehicle);
void drive_differential_stop(void);

typedef struct
{
    void (*init)(void);
    void (*apply)(const VehicleState*);
    void (*stop)(void);
} DriveOps;

static const DriveOps s_ackermann =
{
    .init = drive_ackermann_init,
    .apply = drive_ackermann_apply,
    .stop = drive_ackermann_stop
};

static const DriveOps s_differential =
{
    .init = drive_differential_init,
    .apply = drive_differential_apply,
    .stop = drive_differential_stop
};

static const DriveOps* s_drive = NULL;

void drive_init(void)
{
    switch (BOARD_DRIVE_TYPE)
    {
        case DRIVE_ACKERMANN:
            s_drive = &s_ackermann;
            break;    
        case DRIVE_DIFFERENTIAL:
            s_drive = &s_differential;
            break;
        default:
            s_drive = &s_ackermann;
            break;
    }

    s_drive->init();
}

void drive_apply(const VehicleState* vehicle)
{
    if (s_drive != NULL)
    {
        s_drive->apply(vehicle);
    }
}

void drive_stop(void)
{
    if (s_drive != NULL)
    {
        s_drive->stop();
    }
}
