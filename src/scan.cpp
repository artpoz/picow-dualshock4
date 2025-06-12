#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "btstack.h"

#define MAX_DEVICES 20
enum DEVICE_STATE
{
  REMOTE_NAME_REQUEST,
  REMOTE_NAME_INQUIRED,
  REMOTE_NAME_FETCHED
};
struct device
{
  bd_addr_t address;
  uint8_t pageScanRepetitionMode;
  uint16_t clockOffset;
  enum DEVICE_STATE state;
};

#define INQUIRY_INTERVAL 1
struct device devices[MAX_DEVICES];
int deviceCount = 0;

enum STATE
{
  INIT,
  ACTIVE
};
enum STATE state = INIT;

static btstack_packet_callback_registration_t hci_event_callback_registration;

static int getDeviceIndexForAddress(bd_addr_t addr)
{
  int j;
  for (j = 0; j < deviceCount; j++)
  {
    if (bd_addr_cmp(addr, devices[j].address) == 0)
    {
      return j;
    }
  }
  return -1;
}

static void start_scan(void)
{
  printf("Starting inquiry scan..\n");
  gap_inquiry_start(INQUIRY_INTERVAL);
}

static int has_more_remote_name_requests(void)
{
  int i;
  for (i = 0; i < deviceCount; i++)
  {
    if (devices[i].state == REMOTE_NAME_REQUEST)
      return 1;
  }
  return 0;
}

static void do_next_remote_name_request(void)
{
  int i;
  for (i = 0; i < deviceCount; i++)
  {
    // remote name request
    if (devices[i].state == REMOTE_NAME_REQUEST)
    {
      devices[i].state = REMOTE_NAME_INQUIRED;
      printf("Get remote name of %s...\n", bd_addr_to_str(devices[i].address));
      gap_remote_name_request(devices[i].address, devices[i].pageScanRepetitionMode, devices[i].clockOffset | 0x8000);
      return;
    }
  }
}

static void continue_remote_names(void)
{
  if (has_more_remote_name_requests())
  {
    do_next_remote_name_request();
    return;
  }
  start_scan();
}

char *get_mac(uint8_t packet_type, uint8_t *packet, uint8_t event)
{
  bd_addr_t addr;
  int i;
  int index;

  char *mac = "";

  switch (state)
  {
  case INIT:
  {
    switch (event)
    {
    case BTSTACK_EVENT_STATE:
      if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING)
      {
        start_scan();
        state = ACTIVE;
      }
      break;
    default:
      break;
    }
  }
  break;

  case ACTIVE:
    switch (event)
    {

    case GAP_EVENT_INQUIRY_RESULT:
    {
      if (deviceCount >= MAX_DEVICES)
        break; // already full
      gap_event_inquiry_result_get_bd_addr(packet, addr);
      index = getDeviceIndexForAddress(addr);
      if (index >= 0)
        break; // already in our list

      memcpy(devices[deviceCount].address, addr, 6);
      devices[deviceCount].pageScanRepetitionMode = gap_event_inquiry_result_get_page_scan_repetition_mode(packet);
      devices[deviceCount].clockOffset = gap_event_inquiry_result_get_clock_offset(packet);
      // print info
      char *mac_addr = bd_addr_to_str(addr);
      printf("Device found: %s ", mac_addr);
      printf("with COD: 0x%06x, ", (unsigned int)gap_event_inquiry_result_get_class_of_device(packet));
      printf("pageScan %d, ", devices[deviceCount].pageScanRepetitionMode);
      printf("clock offset 0x%04x", devices[deviceCount].clockOffset);
      if (gap_event_inquiry_result_get_rssi_available(packet))
      {
        printf(", rssi %d dBm", (int8_t)gap_event_inquiry_result_get_rssi(packet));
      }
      if (gap_event_inquiry_result_get_name_available(packet))
      {
        char name_buffer[240];
        int name_len = gap_event_inquiry_result_get_name_len(packet);
        memcpy(name_buffer, gap_event_inquiry_result_get_name(packet), name_len);
        name_buffer[name_len] = 0;
        printf(", name '%s'", name_buffer);
        devices[deviceCount].state = REMOTE_NAME_FETCHED;

        if (strcmp(name_buffer, "Wireless Controller") == 0)
        {
          mac = mac_addr;
        }
      }
      else
      {
        devices[deviceCount].state = REMOTE_NAME_REQUEST;
      }
      printf("\n");
      deviceCount++;
    }
    break;

    case GAP_EVENT_INQUIRY_COMPLETE:
    {
      for (i = 0; i < deviceCount; i++)
      {
        // retry remote name request
        if (devices[i].state == REMOTE_NAME_INQUIRED)
          devices[i].state = REMOTE_NAME_REQUEST;
      }
      continue_remote_names();
    }
    break;

    case HCI_EVENT_REMOTE_NAME_REQUEST_COMPLETE:
    {
      reverse_bd_addr(&packet[3], addr);
      index = getDeviceIndexForAddress(addr);
      if (index >= 0)
      {
        if (packet[2] == 0)
        {
          printf("Name: '%s'\n", &packet[9]);
          devices[index].state = REMOTE_NAME_FETCHED;

          if (strcmp((char const *)&packet[9], "Wireless Controller") == 0)
          {
            mac = bd_addr_to_str(addr);
          }
        }
        else
        {
          printf("Failed to get name: page timeout\n");
        }
      }
      continue_remote_names();
    }
    break;

    default:
      break;
    }
    break;

  default:
    break;
  }

  return mac;
}
