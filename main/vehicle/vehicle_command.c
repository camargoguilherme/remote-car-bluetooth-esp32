#include <string.h>

#include "vehicle/vehicle_command.h"

void vehicle_command_reset(VehicleCommand* cmd)
{
    memset(cmd, 0, sizeof(*cmd));
}
