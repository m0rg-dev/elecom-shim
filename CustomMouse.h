#pragma once

#include "USBHost_t36.h"
#include "CustomHIDInput.h"
#include "CustomHIDParser.h"

class CustomMouse : public CustomHIDInput {
public:
	CustomMouse(USBHost &host) { init(); }
	bool	available() { return mouseEvent; }
	void	mouseDataClear();
	uint8_t getButtons() { return buttons; }
	int     getMouseX() { return mouseX; }
	int     getMouseY() { return mouseY; }
	int     getWheel() { return wheel; }
	int     getWheelH() { return wheelH; }
protected:
	virtual hidclaim_t claim_collection(CustomHIDParser *driver, Device_t *dev, uint32_t topusage);
	virtual void hid_input_begin(uint32_t topusage, uint32_t type, int lgmin, int lgmax);
	virtual void hid_input_data(uint32_t usage, int32_t value);
	virtual void hid_input_end();
	virtual void disconnect_collection(Device_t *dev);

private:
	void init();

	uint8_t collections_claimed = 0;
	volatile bool mouseEvent = false;
	volatile bool hid_input_begin_ = false;
	uint8_t buttons = 0;
	int     mouseX = 0;
	int     mouseY = 0;
	int     wheel = 0;
	int     wheelH = 0;
};
