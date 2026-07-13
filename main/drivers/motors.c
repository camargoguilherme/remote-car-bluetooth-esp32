
#include <stdbool.h>
#include <stddef.h>

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

#include "board/board.h"
#include "common/util.h"
#include "config/config.h"
#include "drivers/motors.h"
#include "drivers/pwm.h"

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

static MotorChannel s_motors[MOTOR_COUNT] = {
	[MOTOR_1] = {
		.channel_a = BOARD_CHANNEL_MOTOR1_A,
		.channel_b = BOARD_CHANNEL_MOTOR1_B,
		.pin_a = BOARD_PIN_MOTOR1_A,
		.pin_b = BOARD_PIN_MOTOR1_B,
		.invert = false,
	},
	[MOTOR_2] = {
		.channel_a = BOARD_CHANNEL_MOTOR2_A,
		.channel_b = BOARD_CHANNEL_MOTOR2_B,
		.pin_a = BOARD_PIN_MOTOR2_A,
		.pin_b = BOARD_PIN_MOTOR2_B,
		.invert = false,
	},
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

static MotorChannel* motor_from_id(MotorId motor)
{
	if (motor < 0 || motor >= MOTOR_COUNT)
		return NULL;

	return &s_motors[motor];
}

static void motor_init(MotorChannel* motor)
{
	motor->last_speed = 0;

	motor_gpio_init(motor->pin_a);
	motor_gpio_init(motor->pin_b);

	ESP_ERROR_CHECK(gpio_set_level(motor->pin_a, 0));
	ESP_ERROR_CHECK(gpio_set_level(motor->pin_b, 0));

	pwm_attach_channel(motor->channel_a, motor->pin_a);
	pwm_attach_channel(motor->channel_b, motor->pin_b);
}

static void motor_set_speed(MotorChannel* motor, int speed_percent)
{	
	if (motor == NULL)
		return;

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
		s_motors[MOTOR_1].invert = config->invert_drive_motor;
		s_motors[MOTOR_2].invert = config->invert_steering_motor;
	}

	for (size_t i = 0; i < MOTOR_COUNT; i++)
		motor_init(&s_motors[i]);

	motors_stop();
}

void motors_set_speed(MotorId motor, int speed_percent)
{
	motor_set_speed(motor_from_id(motor), speed_percent);
}

void motors_stop(void)
{
	for (size_t i = 0; i < MOTOR_COUNT; i++)
		motor_set_speed(&s_motors[i], 0);
}
