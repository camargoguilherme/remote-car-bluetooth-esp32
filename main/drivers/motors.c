
#include <stdbool.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"

#include "config/config.h"
#include "board/board.h"
#include "drivers/motors.h"
#include "vehicle/vehicle.h"
#include "pwm.h"
#include "util.h"

#define TAG "motors"

typedef struct {
	ledc_channel_t pwm_channel;
	gpio_num_t pwm_gpio;
	gpio_num_t in1_gpio;
	gpio_num_t in2_gpio;
	bool invert;
} MotorChannel;

static MotorChannel s_drive_motor = {
	.pwm_channel = BOARD_CHANNEL_DRIVE_MOTOR,
	.pwm_gpio = BOARD_PIN_DRIVE_MOTOR_PWM,
	.in1_gpio = BOARD_PIN_DRIVE_MOTOR_IN1,
	.in2_gpio = BOARD_PIN_DRIVE_MOTOR_IN2,
	.invert = false,
};

static MotorChannel s_steering_motor = {
	.pwm_channel = BOARD_CHANNEL_STEERING_MOTOR,
	.pwm_gpio = BOARD_PIN_STEERING_MOTOR_PWM,
	.in1_gpio = BOARD_PIN_STEERING_MOTOR_IN1,
	.in2_gpio = BOARD_PIN_STEERING_MOTOR_IN2,
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
	motor_gpio_init(motor->in1_gpio);
	motor_gpio_init(motor->in2_gpio);
	ESP_ERROR_CHECK(gpio_set_level(motor->in1_gpio, 0));
	ESP_ERROR_CHECK(gpio_set_level(motor->in2_gpio, 0));
	pwm_attach_channel(motor->pwm_channel, motor->pwm_gpio);
}

static void motor_set_speed(MotorChannel* motor, int speed_percent)
{
	speed_percent = util_clamp_percent(speed_percent);

	if (motor->invert) {
		speed_percent = -speed_percent;
	}

	if (speed_percent == 0) {
		ESP_ERROR_CHECK(gpio_set_level(motor->in1_gpio, 0));
		ESP_ERROR_CHECK(gpio_set_level(motor->in2_gpio, 0));
		pwm_set_duty(motor->pwm_channel, 0);
		return;
	}

	bool forward = speed_percent > 0;
	uint32_t duty = (uint32_t)abs(speed_percent) * pwm_get_max_duty() / 100;

	ESP_ERROR_CHECK(gpio_set_level(motor->in1_gpio, forward ? 1 : 0));
	ESP_ERROR_CHECK(gpio_set_level(motor->in2_gpio, forward ? 0 : 1));
	pwm_set_duty(motor->pwm_channel, duty);
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

	int speed = util_clamp_percent(state->motion.throttle + state->motion.throttle);
	int steering = util_clamp_percent(state->motion.steering);

	motor_set_speed(&s_drive_motor, speed);
	motor_set_speed(&s_steering_motor, steering);
}
