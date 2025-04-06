#include <Arduino.h>
#include "core/tapo/tapo-core.cpp"

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
	SmartBulbAdapter()
	{
		prevMillis = millis();
		core = new L530("192.168.1.111");
	}

	~SmartBulbAdapter()
	{
		delete core;
	}

	void tick()
	{
		core->incrementCookieAge(millis() - prevMillis);
		prevMillis = millis();
	}

	bool turnOn()
	{
		return core->turnOn();
	}

	bool turnOff()
	{
		return core->turnOff();
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