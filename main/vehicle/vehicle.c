#include <stdio.h>
#include <string.h>

#include "vehicle/vehicle.h"

static VehicleState s_vehicle;

void vehicle_init(void)
{
    memset(&s_vehicle, 0, sizeof(s_vehicle));
}

VehicleState* vehicle_get(void)
{
    return &s_vehicle;
}

void apply_motion(const VehicleCommand* command) 
{
    s_vehicle.motion.steering = command->steering;
    s_vehicle.motion.throttle = command->throttle;
}

void apply_lights(const VehicleCommand* command)
{
    // Processa comandos do motorista
    if (command->toggle_hazard)
    {
        s_vehicle.lights.hazard = !s_vehicle.lights.hazard;

        if (s_vehicle.lights.hazard)
        {
            s_vehicle.lights.left_indicator = false;
            s_vehicle.lights.right_indicator = false;
        }
    }

    if (!s_vehicle.lights.hazard)
    {
        /* Seta esquerda */
        if (command->toggle_left_indicator)
        {
            s_vehicle.lights.left_indicator = !s_vehicle.lights.left_indicator;

            if (s_vehicle.lights.left_indicator)
                s_vehicle.lights.right_indicator = false;
        }

        /* Seta direita */
        if (command->toggle_right_indicator)
        {
            s_vehicle.lights.right_indicator = !s_vehicle.lights.right_indicator;

            if (s_vehicle.lights.right_indicator)
                s_vehicle.lights.left_indicator = false;
        }
    }

    /* Farol */
    if (command->toggle_headlight)
    {
        switch (s_vehicle.lights.headlight)
        {
            case HEADLIGHT_OFF:
                s_vehicle.lights.headlight = HEADLIGHT_LOW;
                break;

            case HEADLIGHT_LOW:
                s_vehicle.lights.headlight = HEADLIGHT_MEDIUM;
                break;

            case HEADLIGHT_MEDIUM:
                s_vehicle.lights.headlight = HEADLIGHT_HIGH;
                break;

            default:
                s_vehicle.lights.headlight = HEADLIGHT_OFF;
                break;
        }
    }

    // Atualiza luzes automáticas
    /* Ré */
    s_vehicle.lights.reverse_light = (command->throttle < 0);

    /* Luz de freio */ // TODO: A definir como será acionada a luz de freio, se será por sensor ou por algum comando do gamepad
    s_vehicle.lights.brake_light = false;
}

void apply_horn(const VehicleCommand* command)
{
    s_vehicle.horn.enabled = command->horn_pressed;
}

void vehicle_apply_command(const VehicleCommand* command)
{
    if (command == NULL)
        return;

    /* Movimento */
    apply_motion(command);

    /* Luzes */
    apply_lights(command);

    /* Buzina */
    apply_horn(command);
}

void vehicle_reset(void)
{
    memset(&s_vehicle, 0, sizeof(s_vehicle));

    s_vehicle.lights.headlight = HEADLIGHT_OFF;
    s_vehicle.status.connected = false;
    s_vehicle.status.failsafe = false;
    s_vehicle.status.emergency_stop = false;
}
