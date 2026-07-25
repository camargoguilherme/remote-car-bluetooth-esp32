#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_log.h"

#include "board/board.h"
#include "config/config.h"
#include "drivers/drive/drive.h"
#include "drivers/horn.h"
#include "drivers/lights.h"
#include "drivers/motors.h"
#include "vehicle/vehicle.h"
#include "common/failsafe.h"
#include "app_config.h"

#define TAG "vehicle"

static VehicleState s_vehicle;

static void vehicle_output_task(void* arg)
{
    (void)arg;

    for (;;)
    {
        s_vehicle.status.failsafe_active = failsafe_is_active();

        drive_apply(&s_vehicle);
        lights_apply(&s_vehicle);
        horn_apply(&s_vehicle);

        vTaskDelay(pdMS_TO_TICKS(BOARD_OUTPUT_UPDATE_INTERVAL_MS));
    }
}

static void vehicle_telemetry_task(void* arg)
{
    (void)arg;

    for (;;)
    {
        const VehicleState* vehicle = vehicle_get();
        ESP_LOGI(TAG,
         "Stack livre: %u",
         uxTaskGetStackHighWaterMark(NULL));
        ESP_LOGI(TAG, "Status: Conn=%d, Failsafe=%d, EmergStop=%d",
                 vehicle->status.connected, vehicle->status.failsafe_active, vehicle->status.emergency_stop);
        ESP_LOGI(TAG, "Motion: Steering=%d, Throttle=%d",
                 vehicle->motion.steering, vehicle->motion.throttle);
        ESP_LOGI(TAG, "Lights: Headlight=%d, LeftInd=%d, RightInd=%d, Hazard=%d, Tail=%d, Brake=%d, Reverse=%d",
                 vehicle->lights.headlight, vehicle->lights.left_indicator, vehicle->lights.right_indicator,
                 vehicle->lights.hazard, vehicle->lights.tail_light, vehicle->lights.brake_light,
                 vehicle->lights.reverse_light);
        ESP_LOGI(TAG, "Horn: Enabled=%d", vehicle->horn.enabled);

        vTaskDelay(pdMS_TO_TICKS(BOARD_TELEMETRY_PERIOD_MS));
    }
}

void create_task_vehicle_output(void)
{
    xTaskCreate(vehicle_output_task, "vehicle_output",
        BOARD_TASK_STACK_VEHICLE_OUTPUT,
        NULL,
        BOARD_TASK_PRIORITY_VEHICLE_OUTPUT,
        NULL);
}

void create_task_vehicle_telemetry(void)
{
    xTaskCreate(vehicle_telemetry_task, "vehicle_telemetry",
        BOARD_TASK_STACK_TELEMETRY,
        NULL,
        BOARD_TASK_PRIORITY_TELEMETRY,
        NULL);
}

void vehicle_init(void)
{
    memset(&s_vehicle, 0, sizeof(s_vehicle));
    motors_init(config_get());
    drive_init();
    horn_init();
    lights_init();
    create_task_vehicle_output();
    create_task_vehicle_telemetry();
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
    if (command->headlight_up)
    {
        s_vehicle.lights.tail_light = true;
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
                break;
        }
    }

    if (command->headlight_down)
    {
        switch (s_vehicle.lights.headlight)
        {
            case HEADLIGHT_HIGH:
                s_vehicle.lights.headlight = HEADLIGHT_MEDIUM;
                break;

            case HEADLIGHT_MEDIUM:
                s_vehicle.lights.headlight = HEADLIGHT_LOW;
                break;

            case HEADLIGHT_LOW:
                s_vehicle.lights.headlight = HEADLIGHT_OFF;
                s_vehicle.lights.tail_light = false;
                break;

            default:
                break;
        }
    }

    // Atualiza luzes automáticas
    /* Ré */
    s_vehicle.lights.reverse_light = (command->throttle < 0);

    /* Luz de freio */ // TODO: A definir como será acionada a luz de freio, se será por sensor ou por algum comando do gamepad
    s_vehicle.lights.brake_light = (command->throttle < 0);
}

void apply_horn(const VehicleCommand* command)
{
    s_vehicle.horn.enabled = command->horn_pressed;
}

void vehicle_apply_command(const VehicleCommand* command)
{
    if (command == NULL)
        return;

    apply_motion(command);
    apply_lights(command);
    apply_horn(command);
}

void vehicle_reset(void)
{
    memset(&s_vehicle, 0, sizeof(s_vehicle));

    s_vehicle.status.connected = false;
    s_vehicle.status.failsafe = false;
    s_vehicle.status.emergency_stop = false;
    s_vehicle.status.failsafe_active = false;

    s_vehicle.motion.throttle = 0;
    s_vehicle.motion.steering = 0;

    s_vehicle.lights.headlight = HEADLIGHT_OFF;
    s_vehicle.lights.left_indicator = false;
    s_vehicle.lights.right_indicator = false;
    s_vehicle.lights.hazard = false;
    s_vehicle.lights.tail_light = false;
    s_vehicle.lights.reverse_light = false;
    s_vehicle.lights.brake_light = false;

    s_vehicle.horn.enabled = false;
    
}
