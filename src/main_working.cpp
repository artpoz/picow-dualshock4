#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <functional>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "ds4.h"
#include "arp.h"

using VoidFuncType = void (*)();
using FuncType = std::function<VoidFuncType(int)>;

#define PWM_PIN6 6
#define PWM_PIN20 20

// static int ly = 0;
// static int rx = 0;

class PwmController
{
public:
	static int ly;
	static int ry;
	static int pin6;
	static int pin20;

	static void on_pwm_wrap()
	{
		pwm_clear_irq(pwm_gpio_to_slice_num(pin6));
		pwm_set_gpio_level(pin6, ly * ly);
	}
	
	static void on_pwm_wrap2()
	{
		pwm_clear_irq(pwm_gpio_to_slice_num(pin20));
		pwm_set_gpio_level(pin20, ry * ry);
	}

	static VoidFuncType set_ly(int p)
	{
		pin6 = p;
		return on_pwm_wrap;
	}

	static VoidFuncType set_ry(int p)
	{
		pin20 = p;
		return on_pwm_wrap2;
	}
};
int PwmController::ly = 0;
int PwmController::ry = 0;
int PwmController::pin6 = 0;
int PwmController::pin20 = 0;

/*
void set_up_pwm(int pin, VoidFuncType func)
{
	gpio_set_function(pin, GPIO_FUNC_PWM);
	uint slice_num = pwm_gpio_to_slice_num(pin);
	pwm_clear_irq(slice_num);
	pwm_set_irq_enabled(slice_num, true);
	irq_set_exclusive_handler(PWM_IRQ_WRAP, func);
	irq_set_enabled(PWM_IRQ_WRAP, true);
	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);
	pwm_init(slice_num, &config, true);
}
*/

static inline int8_t clamp8(int16_t value) {
        if (value > 127) {
                return 127;
        } else if (value < -128) {
                return -128;
        }

        return value;
}

int main(void)
{
	ds4_setup();

	//set_up_pwm(PWM_PIN6, PwmController::set_ly(PWM_PIN6));
	//set_up_pwm(PWM_PIN20, PwmController::set_ry(PWM_PIN20));

	struct chassis chassis1 = { 0 };
    struct chassis chassis2 = { 0 };

    chassis_init(&chassis1, 20, 6);
    chassis_init(&chassis2, 19, 7);


	while (true)
	{
		sleep_ms(20);
		tight_loop_contents();
		if (ds4_can_use())
		{
			bt_hid_state state = ds4_get_state();
			printf("buttons: %04x, l: %d,%d, r: %d,%d, l2,r2: %d,%d hat: %d\n", state.buttons, state.lx, state.ly, state.rx, state.ry, state.l2, state.r2, state.hat);
			// pwm_set_gpio_level(PIN_PWM, (state.ry - 128) / 128 * 1023);
			//PwmController::ly = std::max(0, (state.ry - 128) * 2);
			//PwmController::ry = std::max(0, (state.ly - 128) * 2);

			float speed_scale1 = 0.1;
        	float speed_scale2 = 0.05;
			int8_t linear = clamp8(-(state.rx - 128) * speed_scale2);
			int8_t rot = clamp8(-(state.ly - 128) * speed_scale1);

			chassis_set(&chassis1, linear, rot);
        	chassis_set(&chassis2, linear, rot);

		}
	}

	return 0;
}
