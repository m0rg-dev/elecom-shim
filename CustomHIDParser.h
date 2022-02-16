#pragma once

#include <Arduino.h>
#include "USBHost_t36.h"

class CustomHIDInput;

class CustomHIDParser : public USBDriver {
public:
	CustomHIDParser(USBHost &host) : hidTimer(this) { init(); }
	static void driver_ready_for_hid_collection(CustomHIDInput *driver);
	bool sendPacket(const uint8_t *buffer, int cb=-1);
	void setTXBuffers(uint8_t *buffer1, uint8_t *buffer2, uint8_t cb);

	bool sendControlPacket(uint32_t bmRequestType, uint32_t bRequest,
			uint32_t wValue, uint32_t wIndex, uint32_t wLength, void *buf);

	// Atempt for RAWhid and SEREMU to take over processing of data 
	// 
	uint16_t inSize(void) {return in_size;}
	uint16_t outSize(void) {return out_size;}
	void startTimer(uint32_t microseconds) {hidTimer.start(microseconds);}
	void stopTimer() {hidTimer.stop();}
	uint8_t interfaceNumber() { return bInterfaceNumber;}
protected:
	enum { TOPUSAGE_LIST_LEN = 4 };
	enum { USAGE_LIST_LEN = 24 };
	virtual bool claim(Device_t *device, int type, const uint8_t *descriptors, uint32_t len);
	virtual void control(const Transfer_t *transfer);
	virtual void disconnect();
	static void in_callback(const Transfer_t *transfer);
	static void out_callback(const Transfer_t *transfer);
	virtual void timer_event(USBDriverTimer *whichTimer);
	void in_data(const Transfer_t *transfer);
	void out_data(const Transfer_t *transfer);
	bool check_if_using_report_id();
	void parse();
	CustomHIDInput * find_driver(uint32_t topusage);
	void parse(uint16_t type_and_report_id, const uint8_t *data, uint32_t len);
	void init();


	uint8_t activeSendMask(void) {return txstate;} 

private:
	Pipe_t *in_pipe;
	Pipe_t *out_pipe;
	static CustomHIDInput *available_hid_drivers_list;
	//uint32_t topusage_list[TOPUSAGE_LIST_LEN];
	CustomHIDInput *topusage_drivers[TOPUSAGE_LIST_LEN];
	uint16_t in_size;
	uint16_t out_size;
	setup_t setup;
	uint8_t descriptor[800];
	uint8_t report[64];
	uint8_t report2[64];
	uint16_t descsize;
	bool use_report_id;
	Pipe_t mypipes[3] __attribute__ ((aligned(32)));
	Transfer_t mytransfers[5] __attribute__ ((aligned(32)));
	strbuf_t mystring_bufs[1];
	uint8_t txstate = 0;
	uint8_t *tx1 = nullptr;
	uint8_t *tx2 = nullptr;
	bool hid_driver_claimed_control_ = false;
	USBDriverTimer hidTimer;
	uint8_t bInterfaceNumber = 0;
};
