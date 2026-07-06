
#include <stdbool.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

#include "board/board.h"
#include "common/util.h"
#include "config/config.h"
#include "drivers/motors.h"
#include "drivers/pwm.h"
#include "vehicle/vehicle.h"

#define TAG "motors"

typedef struct
{
    ledc_channel_t channel_a;
    ledc_channel_t channel_b;
    gpio_num_t pin_a;
    gpio_num_t pin_b;
		int last_speed;
    bool invert;

} MotorChannel;

static MotorChannel s_drive_motor = {
	.channel_a = BOARD_CHANNEL_DRIVE_MOTOR_A,
	.channel_b = BOARD_CHANNEL_DRIVE_MOTOR_B,
	.pin_a = BOARD_PIN_DRIVE_MOTOR_A,
	.pin_b = BOARD_PIN_DRIVE_MOTOR_B,
	.invert = false,
};

static MotorChannel s_steering_motor = {
	.channel_a = BOARD_CHANNEL_STEERING_MOTOR_A,
	.channel_b = BOARD_CHANNEL_STEERING_MOTOR_B,
	.pin_a = BOARD_PIN_STEERING_MOTOR_A,
	.pin_b = BOARD_PIN_STEERING_MOTOR_B,
	.invert = false,
};

static void motor_gpio_init(gpio_num_t gpio)
{
	gpio_config_t io_conf = {
		.pin_bit_mask = 1ULL << gpio,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_DISABLE,
	};
	ESP_ERROR_CHECK(gpio_config(&io_conf));
}

static void motor_init(MotorChannel* motor)
{
	motor_gpio_init(motor->pin_a);
	motor_gpio_init(motor->pin_b);

	ESP_ERROR_CHECK(gpio_set_level(motor->pin_a, 0));
	ESP_ERROR_CHECK(gpio_set_level(motor->pin_b, 0));

	pwm_attach_channel(motor->channel_a, motor->pin_a);
	pwm_attach_channel(motor->channel_b, motor->pin_b);
}

static void motor_set_speed(MotorChannel* motor, int speed_percent)
{	
	speed_percent = util_clamp_percent(speed_percent);

	if (motor->invert)
		speed_percent = -speed_percent;

	// Não faz nada se a velocidade não mudou
	if (speed_percent == motor->last_speed)
		return;

	motor->last_speed = speed_percent;

	uint32_t duty = util_percent_to_duty(speed_percent, pwm_get_max_duty());

	if (speed_percent == 0)
	{
			pwm_set_duty(motor->channel_a, 0);
			pwm_set_duty(motor->channel_b, 0);
			return;
	}

	if (speed_percent > 0)
	{
			pwm_set_duty(motor->channel_a, duty);
			pwm_set_duty(motor->channel_b, 0);
	}
	else
	{
			pwm_set_duty(motor->channel_a, 0);
			pwm_set_duty(motor->channel_b, duty);
	}
}

void motors_init(const AppConfig* config)
{
	pwm_init();

	if (config != NULL) {
		s_drive_motor.invert = config->invert_drive_motor;
		s_steering_motor.invert = config->invert_steering_motor;
	}

	motor_init(&s_drive_motor);
	motor_init(&s_steering_motor);
	motors_stop();
}

void motors_stop(void)
{
	motor_set_speed(&s_drive_motor, 0);
	motor_set_speed(&s_steering_motor, 0);
}

void motors_apply(const VehicleState* state)
{
	if (state == NULL) {
			motors_stop();
			return;
	}

	int speed = util_clamp_percent(state->motion.throttle);
	int steering = util_clamp_percent(state->motion.steering);

	motor_set_speed(&s_drive_motor, speed);
	motor_set_speed(&s_steering_motor, steering);
}
