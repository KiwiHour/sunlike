#ifndef SUNLIKE_CONTROLLER
#define SUNLIKE_CONTROLLER

#include "core/smart_bulb_adapter.cpp"

class SunlikeController
{
private:
	SmartBulbAdapter *bulb;

public:
	SunlikeController(SmartBulbAdapter *adapter)
	{
		bulb = adapter;
	}

	void tick(bool erm)
	{
		Serial.println("erm, what the tick?");
		if (erm)
			bulb->turnOff();
	}
};

#endif