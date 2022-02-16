// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  HID driver for ELECOM devices:
 *  - BM084 Bluetooth Mouse
 *  - EX-G Trackballs (M-XT3DRBK, M-XT3URBK, M-XT4DRBK)
 *  - DEFT Trackballs (M-DT1DRBK, M-DT1URBK, M-DT2DRBK, M-DT2URBK)
 *  - HUGE Trackballs (M-HT1DRBK, M-HT1URBK)
 *
 *  Copyright (c) 2010 Richard Nauber <Richard.Nauber@gmail.com>
 *  Copyright (c) 2016 Yuxuan Shui <yshuiv7@gmail.com>
 *  Copyright (c) 2017 Diego Elio Pettenò <flameeyes@flameeyes.eu>
 *  Copyright (c) 2017 Alex Manoussakis <amanou@gnu.org>
 *  Copyright (c) 2017 Tomasz Kramkowski <tk@the-tk.com>
 *  Copyright (c) 2020 YOSHIOKA Takuma <lo48576@hard-wi.red>
 */

#include "CustomHIDParser.h"
#include <Arduino.h>

#define USB_VENDOR_ID_ELECOM 0x056e
#define USB_DEVICE_ID_ELECOM_BM084 0x0061
#define USB_DEVICE_ID_ELECOM_M_XGL20DLBK 0x00e6
#define USB_DEVICE_ID_ELECOM_M_XT3URBK 0x00fb
#define USB_DEVICE_ID_ELECOM_M_XT3DRBK 0x00fc
#define USB_DEVICE_ID_ELECOM_M_XT4DRBK 0x00fd
#define USB_DEVICE_ID_ELECOM_M_DT1URBK 0x00fe
#define USB_DEVICE_ID_ELECOM_M_DT1DRBK 0x00ff
#define USB_DEVICE_ID_ELECOM_M_HT1URBK 0x010c
#define USB_DEVICE_ID_ELECOM_M_HT1DRBK 0x010d

#define MOUSE_BUTTONS_MAX 8
static void
mouse_button_fixup(CustomHIDParser *hdev, uint8_t *rdesc, unsigned int rsize,
                   unsigned int button_bit_count, unsigned int padding_bit,
                   unsigned int button_report_size,
                   unsigned int button_usage_maximum, int nbuttons) {
  if (rsize < 32 || rdesc[button_bit_count] != 0x95 ||
      rdesc[button_report_size] != 0x75 ||
      rdesc[button_report_size + 1] != 0x01 ||
      rdesc[button_usage_maximum] != 0x29 || rdesc[padding_bit] != 0x75)
    return;
  Serial.printf("Fixing up Elecom mouse button count\n");
  // nbuttons = clamp(nbuttons, 0, MOUSE_BUTTONS_MAX);
  rdesc[button_bit_count + 1] = nbuttons;
  rdesc[button_usage_maximum + 1] = nbuttons;
  rdesc[padding_bit + 1] = MOUSE_BUTTONS_MAX - nbuttons;
}

static uint8_t *elecom_report_fixup(CustomHIDParser *hdev, uint8_t *rdesc,
                                    uint16_t *rsize) {
  switch (hdev->idProduct()) {
  case USB_DEVICE_ID_ELECOM_BM084:
    /* The BM084 Bluetooth mouse includes a non-existing horizontal
     * wheel in the HID descriptor. */
    if (*rsize >= 48 && rdesc[46] == 0x05 && rdesc[47] == 0x0c) {
      Serial.printf("Fixing up Elecom BM084 report descriptor\n");
      rdesc[47] = 0x00;
    }
    break;
  case USB_DEVICE_ID_ELECOM_M_XGL20DLBK:
    /*
     * Report descriptor format:
     * 20: button bit count
     * 28: padding bit count
     * 22: button report size
     * 14: button usage maximum
     */
    mouse_button_fixup(hdev, rdesc, *rsize, 20, 28, 22, 14, 8);
    break;
  case USB_DEVICE_ID_ELECOM_M_XT3URBK:
  case USB_DEVICE_ID_ELECOM_M_XT3DRBK:
  case USB_DEVICE_ID_ELECOM_M_XT4DRBK:
    /*
     * Report descriptor format:
     * 12: button bit count
     * 30: padding bit count
     * 14: button report size
     * 20: button usage maximum
     */
    mouse_button_fixup(hdev, rdesc, *rsize, 12, 30, 14, 20, 6);
    break;
  case USB_DEVICE_ID_ELECOM_M_DT1URBK:
  case USB_DEVICE_ID_ELECOM_M_DT1DRBK:
  case USB_DEVICE_ID_ELECOM_M_HT1URBK:
  case USB_DEVICE_ID_ELECOM_M_HT1DRBK:
    /*
     * Report descriptor format:
     * 12: button bit count
     * 30: padding bit count
     * 14: button report size
     * 20: button usage maximum
     */
    mouse_button_fixup(hdev, rdesc, *rsize, 12, 30, 14, 20, 8);
    break;
  }
  return rdesc;
}
