#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#include "ds4.h"

int main(void)
{
	ds4_setup();

	for (;;)
	{
		sleep_ms(20);
		if (ds4_can_use())
		{
			bt_hid_state state = ds4_get_state();
			printf("buttons: %04x, l: %d,%d, r: %d,%d, l2,r2: %d,%d hat: %d\n", state.buttons, state.lx, state.ly, state.rx, state.ry, state.l2, state.r2, state.hat);
		}
	}

	return 0;
}
