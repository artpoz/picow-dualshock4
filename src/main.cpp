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
#include "bt_hid.h"
#include "mac_store.h"

const uint BUTTON_PIN = 15;
const uint LED_PIN = 25;

static inline int8_t clamp8(int16_t value) {
        if (value > 127) {
                return 127;
        } else if (value < -128) {
                return -128;
        }

        return value;
}

void test()
{
    printf("Hello World\n");
	sleep_ms(1000);
}



int main(void)
{
    
	stdio_init_all();

    sleep_ms(2000);

    if (is_mac_saved()) 
    {

        uint8_t odczytany_mac[MAC_ADDRESS_SIZE];
        read_mac(odczytany_mac);

    } else {
        printf("Adres MAC nie został zapisany.\n");
    }

	sleep_ms(1000);	
    
	ds4_setup();

	struct chassis chassis1 = { 0 };
    struct chassis chassis2 = { 0 };

    chassis_init(&chassis1, 20, 6);
    chassis_init(&chassis2, 19, 7);


    printf("WHILE\n");
    bool button_pressed = false;
    bool exit = false;

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);  

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

	while (!exit)
	{
		sleep_ms(20);        
		tight_loop_contents();        
		if (ds4_can_use())
		{
			bt_hid_state state = ds4_get_state();
			//printf("buttons: %04x, l: %d,%d, r: %d,%d, l2,r2: %d,%d hat: %d\n", state.buttons, state.lx, state.ly, state.rx, state.ry, state.l2, state.r2, state.hat);
			   
			float speed_scale1 = 0.1;
        	float speed_scale2 = 0.025;  // 0.05
			int8_t linear = clamp8(-(state.rx - 128) * speed_scale2);
			int8_t rot = clamp8(-(state.ly - 128) * speed_scale1);

			chassis_set(&chassis1, linear, rot);
        	chassis_set(&chassis2, linear, rot);

		}  

        bool button_pressed = !gpio_get(BUTTON_PIN); 
        if (button_pressed)
        {
                uint8_t mac[MAC_ADDRESS_SIZE] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
                save_mac(mac,false);
                gpio_put(LED_PIN, button_pressed);  
        }

	}        
   
	return 0;
}
