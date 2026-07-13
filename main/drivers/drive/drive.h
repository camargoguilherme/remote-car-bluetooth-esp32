#pragma once

typedef struct VehicleState VehicleState;

void drive_init(void);
void drive_apply(const VehicleState* vehicle);
void drive_stop(void);
