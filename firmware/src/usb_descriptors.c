#include <string.h>

#include "tusb.h"

#define USBD_VID 0x1209
#define USBD_PID 0x5750

#define ITF_NUM_CDC 0
#define ITF_NUM_CDC_DATA 1
#define ITF_NUM_HID 2
#define ITF_NUM_TOTAL 3

#define EPNUM_CDC_NOTIF 0x81
#define EPNUM_CDC_OUT 0x02
#define EPNUM_CDC_IN 0x82
#define EPNUM_HID 0x83

#define DESC_STR_MAX 32

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_HID_DESC_LEN)

enum {
  USBD_STR_MANUF = 1,
  USBD_STR_PRODUCT,
  USBD_STR_CDC,
  USBD_STR_HID,
};

static const tusb_desc_device_t usbd_desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = USBD_VID,
    .idProduct          = USBD_PID,
    .bcdDevice          = 0x0100,
    .iManufacturer      = USBD_STR_MANUF,
    .iProduct           = USBD_STR_PRODUCT,
    .bNumConfigurations = 1,
};

static const uint8_t hid_report_desc[] = {
    0x05, 0x01,       // Usage Page (Desktop)
    0x09, 0x05,       // Usage (Gamepad)
    0xA1, 0x01,       // Collection (Application)
    0x05, 0x09,       //   Usage Page (Button)
    0x19, 0x01,       //   Usage Minimum (Button 1)
    0x29, 0x0C,       //   Usage Maximum (Button 12)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x95, 0x0C,       //   Report Count (12)
    0x75, 0x01,       //   Report Size (1)
    0x81, 0x02,       //   Input (Variable)
    0x95, 0x04,       //   Report Count (4)
    0x75, 0x01,       //   Report Size (1)
    0x81, 0x03,       //   Input (Constant, Variable)
    0x05, 0x01,       //   Usage Page (Desktop)
    0x09, 0x30,       //   Usage (X)
    0x09, 0x31,       //   Usage (Y)
    0x09, 0x32,       //   Usage (Z)
    0x09, 0x33,       //   Usage (Rx)
    0x09, 0x34,       //   Usage (Ry)
    0x09, 0x35,       //   Usage (Rz)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x06,       //   Report Count (6)
    0x81, 0x02,       //   Input (Variable)
    0xC0              // End Collection
};

static const char *usbd_desc_str[] = {
    [USBD_STR_MANUF]   = "WavePhoenix",
    [USBD_STR_PRODUCT] = "USB Gamepad",
    [USBD_STR_CDC]     = "CDC Interface",
    [USBD_STR_HID]     = "HID Interface",
};

static const uint8_t usbd_desc_cfg[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, USBD_STR_CDC, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, USBD_STR_HID, HID_ITF_PROTOCOL_NONE, sizeof(hid_report_desc), EPNUM_HID, 64, 5)};

uint8_t const *tud_hid_descriptor_report_cb(uint8_t itf)
{
  return hid_report_desc;
}

uint8_t const *tud_descriptor_device_cb(void)
{
  return (uint8_t const *)&usbd_desc_device;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
  return usbd_desc_cfg;
}

static uint8_t copy_str_to_utf16(uint16_t *dst, const char *src, uint8_t max_chars)
{
  uint8_t len = 0;
  while (src[len] && len < max_chars) {
    dst[len] = src[len];
    len++;
  }
  return len;
}

const uint16_t *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  static uint16_t desc_str[DESC_STR_MAX];

  if (index == 0) {
    desc_str[1] = 0x0409;
    desc_str[0] = (TUSB_DESC_STRING << 8) | 4;
    return desc_str;
  }

  if (index >= (sizeof(usbd_desc_str) / sizeof(usbd_desc_str[0])))
    return NULL;

  uint8_t len = copy_str_to_utf16(&desc_str[1], usbd_desc_str[index], DESC_STR_MAX - 1);
  desc_str[0] = (TUSB_DESC_STRING << 8) | ((len * 2) + 2);

  return desc_str;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen)
{

  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, const uint8_t *buffer,
                           uint16_t bufsize)
{
}
