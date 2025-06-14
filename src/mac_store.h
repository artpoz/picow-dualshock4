#pragma once
#include <stdint.h>

#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)  // last page of flash memory
#define MAC_ADDRESS_SIZE 6
#define MAC_WRITTEN_FLAG 0xA5
#define MAC_EMPTY_FLAG 0x00

void save_mac(const uint8_t *mac_address,bool flag);
void read_mac(uint8_t *mac_address);
bool is_mac_saved();

