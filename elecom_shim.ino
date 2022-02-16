#include "CustomHIDParser.h"
#include "CustomMouse.h"
#include "USBHost_t36.h"

USBHost host;
CustomHIDParser hid1(host);
CustomMouse mouse(host);

void setup() { host.begin(); }

void loop() {
  host.Task();

  if (mouse.available()) {
    // Windows doesn't like HID mouse10-mouse12 either, so we'll strip those off
    // here and report them as F13-F15.
    usb_mouse_buttons_state = mouse.getButtons() & 0x1F;
    Mouse.move(mouse.getMouseX(), mouse.getMouseY(), mouse.getWheel(),
               mouse.getWheelH());

    if (mouse.getButtons() & 0x20) {
      Keyboard.set_key1(KEY_F13);
    } else {
      Keyboard.set_key1(0);
    }

    if (mouse.getButtons() & 0x40) {
      Keyboard.set_key2(KEY_F14);
    } else {
      Keyboard.set_key2(0);
    }

    if (mouse.getButtons() & 0x80) {
      Keyboard.set_key3(KEY_F15);
    } else {
      Keyboard.set_key3(0);
    }

    Keyboard.send_now();

    mouse.mouseDataClear();
  }
}
