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

//#define BUTTON_A 1
/*
void save()
{
	int err = lfs_mount(&lfs, &cfg);
    if (err) {
        // Formatowanie pamięci, jeśli montowanie się nie powiedzie
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }

    // Zapis do pliku
    lfs_file_open(&lfs, &file, "hello.txt", LFS_O_WRONLY | LFS_O_CREAT);
    const char *message = "Hello, Raspberry Pi Pico!";
    lfs_file_write(&lfs, &file, message, strlen(message));
    lfs_file_close(&lfs, &file);

    // Odczyt z pliku
    lfs_file_open(&lfs, &file, "hello.txt", LFS_O_RDONLY);
    char buffer[256];
    lfs_file_read(&lfs, &file, buffer, sizeof(buffer));
    lfs_file_close(&lfs, &file);

    // Wyświetlenie odczytanych danych
    printf("Odczytano z pliku: %s\n", buffer);

    // Odmontowanie systemu plików
    lfs_unmount(&lfs);
}
*/
int main(void)
{


//****** ten fragment dziala */
/*
    stdio_init_all();
	sleep_ms(5000);

    
    uint8_t mac[MAC_ADDRESS_SIZE] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    zapisz_mac(mac);

    if (czy_mac_zapisany()) 
    {

        uint8_t odczytany_mac[MAC_ADDRESS_SIZE];
        odczytaj_mac(odczytany_mac);

        // Wyświetlenie adresu MAC
        for(int i = 0; i < MAC_ADDRESS_SIZE; i++) {
            printf("%02X", odczytany_mac[i]);
            if (i < MAC_ADDRESS_SIZE - 1) {
                printf(":");
            }
        }
        printf("\n");
    } else {
        printf("Adres MAC nie został zapisany.\n");
    }

    while(1)
	{
        test();    
    }
    */
//*************************************************** */
    
	stdio_init_all();
	sleep_ms(2000);
	
    
	ds4_setup();

    //uint8_t mac[MAC_ADDRESS_SIZE] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    //zapisz_mac(mac);

	//set_up_pwm(PWM_PIN6, PwmController::set_ly(PWM_PIN6));
	//set_up_pwm(PWM_PIN20, PwmController::set_ry(PWM_PIN20));

	struct chassis chassis1 = { 0 };
    struct chassis chassis2 = { 0 };

    chassis_init(&chassis1, 20, 6);
    chassis_init(&chassis2, 19, 7);


    printf("WHILE\n");
    bool button_pressed = false;
    bool exit = false;
	while (!exit)
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

            //if (state.buttons & BUTTON_A & !button_pressed) {			
			//    printf("Button A is pressed\n");
            //    exit = true;
            //    button_pressed = true;                
		    //}   

			float speed_scale1 = 0.1;
        	float speed_scale2 = 0.025;  // 0.05
			int8_t linear = clamp8(-(state.rx - 128) * speed_scale2);
			int8_t rot = clamp8(-(state.ly - 128) * speed_scale1);

			chassis_set(&chassis1, linear, rot);
        	chassis_set(&chassis2, linear, rot);

		}        
	}    
    
    //char *mac = "";
    //mac = get_addr_string();
    //printf("MAC: %s\n", mac);
    //uint8_t *mac2 = reinterpret_cast<uint8_t*>(mac);
    /*
    uint8_t mac2[MAC_ADDRESS_SIZE] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    zapisz_mac(mac2);
    for(int i = 0; i < MAC_ADDRESS_SIZE; i++) 
    {
        printf("%02X", mac2[i]);
        if (i < MAC_ADDRESS_SIZE - 1) {
            printf(":");
        }
         printf("\n");
    }
    while(1)
	{
		test();
	}
    */
/*
    if (mac[0] != '\0')
    {       
         zapisz_mac(reinterpret_cast<const uint8_t*>(mac));
         const uint8_t* tab = reinterpret_cast<const uint8_t*>(mac); 
         printf("MAC SAVED", mac);
         for(int i = 0; i < MAC_ADDRESS_SIZE; i++) {
            printf("%02X", tab[i]);
            if (i < MAC_ADDRESS_SIZE - 1) {
                printf(":");
            }
        }
        printf("\n");
    }
*/
	return 0;
}
