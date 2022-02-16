# Hardware shim for function buttons on certain Elecom trackballs

Simple project based on the Teensy 4.0 to remap the Fn1-Fn3 keys on my Elecom
trackball mouse to F13-F15. Definitely works on the M-HT1DRBK (which is what I
have), probably works on anything supported by the Linux kernel (M-XGL20DLBK,
M-XT3URBK, M-XT3DRBK, M-XT4DRBK, M-DT1URBK, M-DT1DRBK, M-HT1URBK, M-HT1DRBK).

Upload to your Teensy 4.0 with Teensyduino (Serial + Keyboard + Mouse +
Joystick, any reasonable CPU frequency will do), solder a USB port or cut-up USB
cable on the USB Host pads on the back, and plug the Elecom receiver into the
Teensy and the Teensy into your computer. Plugging the receiver into a
powered-up Teensy probably also works, but there's no reason to and it may cause
transient power conditions that exceed the USB specification. Plugging any other
HID mouse into the downstream USB port will pass it through to the host computer
(but why would you do that).

Remapping the keys involved can be done by modifying the keys called out in
`elecom_shim.ino`, though the real intention is just that you'll pipe them
through to AHK or something and do your configuration in software. You could
also modify `elecom_shim.ino` to pass through all the mouse buttons to the host
computer without a whole lot of trouble, but as far as I can tell most Windows
and Mac software can't deal with mouse buttons past mouse9 anyways (and if
you're on Linux you don't need this device to begin with).

As far as how it works technically, the Elecom trackballs in question here
essentially work like any other USB mouse, they just misrepresent their button
counts to the host computer and force you to use Elecom's proprietary driver to
listen for the extra buttons anyways. Thankfully, the Linux kernel already
figured that one out for me, so this code is just a modification of the Teensy
USB stack set up to run Linux's "fix the HID report descriptor" code when you
plug in an Elecom device, plus a little sugar on top to remap mouse10-mouse12 to
F13-F15.
