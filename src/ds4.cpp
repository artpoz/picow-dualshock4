#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "bt_hid.h"

void ds4_setup()
{
  stdio_init_all();

  sleep_ms(2000);
  printf("Hello\n");

  multicore_launch_core1(bt_main);
  
}

struct bt_hid_state hid_state;

bool ds4_can_use()
{
  bool can_use = bt_hid_get_can_use();

  if (can_use)
  {    
    bt_hid_get_latest(&hid_state);
  }

  return can_use;
}

bt_hid_state ds4_get_state()
{
  return hid_state;
}
