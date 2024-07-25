#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <functional>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "ds4.h"

using VoidFuncType = void (*)();
using FuncType = std::function<VoidFuncType(int)>;

#define PWM_PIN 16

// static int ly = 0;
// static int rx = 0;

class PwmController
{
public:
	static int ly;
	static int pin;

	static void on_pwm_wrap()
	{
		pwm_clear_irq(pwm_gpio_to_slice_num(pin));
		pwm_set_gpio_level(pin, ly * ly);
	}

	static VoidFuncType set_ly(int p)
	{
		pin = p;
		return on_pwm_wrap;
	}
};
int PwmController::ly = 0;
int PwmController::pin = 0;

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

int main(void)
{
	ds4_setup();

	set_up_pwm(PWM_PIN, PwmController::set_ly(PWM_PIN));

	while (true)
	{
		sleep_ms(20);
		tight_loop_contents();
		if (ds4_can_use())
		{
			bt_hid_state state = ds4_get_state();
			printf("buttons: %04x, l: %d,%d, r: %d,%d, l2,r2: %d,%d hat: %d\n", state.buttons, state.lx, state.ly, state.rx, state.ry, state.l2, state.r2, state.hat);
			// pwm_set_gpio_level(PIN_PWM, (state.ry - 128) / 128 * 1023);
			PwmController::ly = std::max(0, (state.ry - 128) * 2);
		}
	}

	return 0;
}
