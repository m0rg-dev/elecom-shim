#include <Arduino.h>

#include "CustomHIDParser.h"
#include "CustomMouse.h"

void CustomMouse::init() {
  CustomHIDParser::driver_ready_for_hid_collection(this);
}

hidclaim_t CustomMouse::claim_collection(CustomHIDParser *driver, Device_t *dev,
                                         uint32_t topusage) {
  // only claim Desktop/Mouse
  if ((topusage != 0x10002) && (topusage != 0x10001))
    return CLAIM_NO;
  // only claim from one physical device
  if (mydevice != NULL && dev != mydevice)
    return CLAIM_NO;
  mydevice = dev;
  collections_claimed++;
  return CLAIM_REPORT;
}

void CustomMouse::disconnect_collection(Device_t *dev) {
  if (--collections_claimed == 0) {
    mydevice = NULL;
  }
}

void CustomMouse::hid_input_begin(uint32_t topusage, uint32_t type, int lgmin,
                                  int lgmax) {
  // TODO: check if absolute coordinates
  hid_input_begin_ = true;
}

void CustomMouse::hid_input_data(uint32_t usage, int32_t value) {
  // Serial.printf("Mouse: usage=%X, value=%d\n", usage, value);
  uint32_t usage_page = usage >> 16;
  usage &= 0xFFFF;
  if (usage_page == 9 && usage >= 1 && usage <= 8) {
    if (value == 0) {
      buttons &= ~(1 << (usage - 1));
    } else {
      buttons |= (1 << (usage - 1));
    }
  } else if (usage_page == 1) {
    switch (usage) {
    case 0x30:
      mouseX = value;
      break;
    case 0x31:
      mouseY = value;
      break;
    case 0x32: // Apple uses this for horizontal scroll
      wheelH = value;
      break;
    case 0x38:
      wheel = value;
      break;
    }
  } else if (usage_page == 12) {
    if (usage == 0x238) { // Microsoft uses this for horizontal scroll
      wheelH = value;
    }
  }
}

void CustomMouse::hid_input_end() {
  if (hid_input_begin_) {
    mouseEvent = true;
    hid_input_begin_ = false;
  }
}

void CustomMouse::mouseDataClear() {
  mouseEvent = false;
  buttons = 0;
  mouseX = 0;
  mouseY = 0;
  wheel = 0;
  wheelH = 0;
}
