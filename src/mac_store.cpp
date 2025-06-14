#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "mac_store.h"
#include "pico/multicore.h"
#include <cstring>

#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)

void save_mac(const uint8_t *mac_address, bool flag) {
    
    uint32_t ints = save_and_disable_interrupts();

    uint8_t flash_data[FLASH_SECTOR_SIZE];
    memcpy(flash_data, (const void *)(XIP_BASE + FLASH_TARGET_OFFSET), FLASH_SECTOR_SIZE);

    if (flag)
        flash_data[FLASH_SECTOR_SIZE - MAC_ADDRESS_SIZE - 1] = MAC_WRITTEN_FLAG;
    else
        flash_data[FLASH_SECTOR_SIZE - MAC_ADDRESS_SIZE - 1] = MAC_EMPTY_FLAG;

    memcpy(flash_data + FLASH_SECTOR_SIZE - MAC_ADDRESS_SIZE, mac_address, MAC_ADDRESS_SIZE);

    multicore_lockout_start_blocking();

    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);

    flash_range_program(FLASH_TARGET_OFFSET, flash_data, FLASH_SECTOR_SIZE);

    multicore_lockout_end_blocking();

    restore_interrupts(ints);

    printf("SAVED\n");

     for(int i = 0; i < MAC_ADDRESS_SIZE; i++) {
            printf("%02X", mac_address[i]);
            if (i < MAC_ADDRESS_SIZE - 1) {
                printf(":");
            }
        }
}

void read_mac(uint8_t *mac_address) {
   
    memcpy(mac_address, (const void *)(XIP_BASE + FLASH_TARGET_OFFSET + FLASH_SECTOR_SIZE - MAC_ADDRESS_SIZE), MAC_ADDRESS_SIZE);

    printf("LOAD\n");
    
     for(int i = 0; i < MAC_ADDRESS_SIZE; i++) {
            printf("%02X", mac_address[i]);
            if (i < MAC_ADDRESS_SIZE - 1) {
                printf(":");
            }
        }
}

bool is_mac_saved() {
    uint8_t flag = *(const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET + FLASH_SECTOR_SIZE - MAC_ADDRESS_SIZE - 1);
    return (flag == MAC_WRITTEN_FLAG);
}