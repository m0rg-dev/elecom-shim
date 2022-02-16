#pragma once

#include <Arduino.h>
#include "USBHost_t36.h"

class CustomHIDParser;

class CustomHIDInput {
public:
	operator bool() { return (mydevice != nullptr); }
	uint16_t idVendor() { return (mydevice != nullptr) ? mydevice->idVendor : 0; }
	uint16_t idProduct() { return (mydevice != nullptr) ? mydevice->idProduct : 0; }
	const uint8_t *manufacturer()
		{  return  ((mydevice == nullptr) || (mydevice->strbuf == nullptr)) ? nullptr : &mydevice->strbuf->buffer[mydevice->strbuf->iStrings[strbuf_t::STR_ID_MAN]]; }
	const uint8_t *product()
		{  return  ((mydevice == nullptr) || (mydevice->strbuf == nullptr)) ? nullptr : &mydevice->strbuf->buffer[mydevice->strbuf->iStrings[strbuf_t::STR_ID_PROD]]; }
	const uint8_t *serialNumber()
		{  return  ((mydevice == nullptr) || (mydevice->strbuf == nullptr)) ? nullptr : &mydevice->strbuf->buffer[mydevice->strbuf->iStrings[strbuf_t::STR_ID_SERIAL]]; }


private:
	virtual hidclaim_t claim_collection(CustomHIDParser *driver, Device_t *dev, uint32_t topusage);
	virtual bool hid_process_in_data(const Transfer_t *transfer) {return false;}
	virtual bool hid_process_out_data(const Transfer_t *transfer) {return false;}
	virtual bool hid_process_control(const Transfer_t *transfer) {return false;}
	virtual void hid_input_begin(uint32_t topusage, uint32_t type, int lgmin, int lgmax);
	virtual void hid_input_data(uint32_t usage, int32_t value);
	virtual void hid_input_end();
	virtual void disconnect_collection(Device_t *dev);
	virtual void hid_timer_event(USBDriverTimer *whichTimer) { }
	void add_to_list();
	CustomHIDInput
 *next = NULL;
	friend class CustomHIDParser;
protected:
	Device_t *mydevice = NULL;
};

