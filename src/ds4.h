/*struct bt_hid_state
{
  uint16_t buttons;
  uint8_t lx;
  uint8_t ly;
  uint8_t rx;
  uint8_t ry;
  uint8_t l2;
  uint8_t r2;
  uint8_t hat;
  uint8_t pad;
};
*/
#include "bt_hid.h"

void ds4_setup();
bool ds4_can_use();
bt_hid_state ds4_get_state();
