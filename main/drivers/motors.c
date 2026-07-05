
#include <stdbool.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"

#include "config/config.h"
#include "drivers/motors.h"
#include "vehicle/vehicle.h"
#include "pwm.h"
#include "util.h"

#define TAG "motors"

#define LEFT_MOTOR_PWM_GPIO 18
#define LEFT_MOTOR_IN1_GPIO 19
#define LEFT_MOTOR_IN2_GPIO 21

#define RIGHT_MOTOR_PWM_GPIO 5
#define RIGHT_MOTOR_IN1_GPIO 25
#define RIGHT_MOTOR_IN2_GPIO 26

typedef struct {
	ledc_channel_t pwm_channel;
	gpio_num_t pwm_gpio;
	gpio_num_t in1_gpio;
	gpio_num_t in2_gpio;
	bool invert;
} MotorChannel;

static MotorChannel s_left_motor = {
	.pwm_channel = LEDC_CHANNEL_0,
	.pwm_gpio = LEFT_MOTOR_PWM_GPIO,
	.in1_gpio = LEFT_MOTOR_IN1_GPIO,
	.in2_gpio = LEFT_MOTOR_IN2_GPIO,
	.invert = false,
};

static MotorChannel s_right_motor = {
	.pwm_channel = LEDC_CHANNEL_1,
	.pwm_gpio = RIGHT_MOTOR_PWM_GPIO,
	.in1_gpio = RIGHT_MOTOR_IN1_GPIO,
	.in2_gpio = RIGHT_MOTOR_IN2_GPIO,
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
		s_left_motor.invert = config->invert_left_motor;
		s_right_motor.invert = config->invert_right_motor;
	}

	motor_init(&s_left_motor);
	motor_init(&s_right_motor);
	motors_stop();
}

void motors_stop(void)
{
	motor_set_speed(&s_left_motor, 0);
	motor_set_speed(&s_right_motor, 0);
}

void motors_apply(const VehicleState* state)
{
	int left;
	int right;

	if (state == NULL) {
			motors_stop();
			return;
	}

	left = util_clamp_percent(state->motion.throttle + state->motion.steering);
	right = util_clamp_percent(state->motion.throttle - state->motion.steering);

	motor_set_speed(&s_left_motor, left);
	motor_set_speed(&s_right_motor, right);
}
