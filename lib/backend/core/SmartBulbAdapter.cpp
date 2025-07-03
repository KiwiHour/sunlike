#ifndef SMART_BULB_ADAPTER
#define SMART_BULB_ADAPTER

#include <Arduino.h>
#include "tapo/tapo-core.cpp"

// Intermediate adapter to generalise functions for the rest of the program
// You can change the method implementation to match your bulb API of choice
class SmartBulbAdapter
{
private:
	L530 *core;
	unsigned long prevMillis;

	bool errorCodeToBool(int errorCode)
	{
		return errorCode == 0;
	}

public:
	~SmartBulbAdapter()
	{
		delete core;
	}

	void begin()
	{
		prevMillis = millis();
		core = new L530("192.168.1.203");
	}

	void tick()
	{
		core->incrementCookieAge(millis() - prevMillis);
		prevMillis = millis();
	}

	// 0 for off, 1 for on
	int getPowerState()
	{
		return (int)(core->isOn());
	}

	bool setPowerState(int state)
	{
		if (state == 1)
			core->turnOn();
		else if (state == 0)
			core->turnOff();
		else
			return false;

		return true;
	}

	int getBrightness()
	{
		return core->getBrightness();
	}

	bool setBrightness(int brightness)
	{
		return errorCodeToBool(core->setBrightness(brightness));
	}

	int getColorTemperature()
	{
		return core->getColorTemperature();
	}

	bool setColorTemperature(int temperature)
	{
		return errorCodeToBool(core->setColorTemperature(temperature));
	}

	int getHue()
	{
		return core->getHue();
	}

	bool setHue(int hue)
	{
		return errorCodeToBool(core->setHue(hue));
	}

	int getSaturation()
	{
		return core->getSaturation();
	}

	bool setSaturation(int saturation)
	{
		return errorCodeToBool(core->setSaturation(saturation));
	}
};

#endif