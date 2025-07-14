#ifndef SMART_BULB_ADAPTER
#define SMART_BULB_ADAPTER

#include <Arduino.h>
#include "tapo/tapo-core.cpp"

// TODO: possible fix for O(n) loading time (should be O(1))
// Add a getAllAttributes method which just runs the getDeviceInfo from the tapo-core (will have to change visibility to public)
// You then need to setup a state group, which means they all share the same fetcher
// and when state.fetch is run, for independent values, it works as normal,
// but for groups, the group implementation takes over and the fetch just runs getAllAttributes once and updates the internal values
// OR!!!!!
// create a snapshotting system
// store a AttributeSnapshot struct (or maybe a different name, attribute sounds weird) with all the values of the bulb
// along with a snapshotMillis that stores when the most recent snapshot was made
// do SmartBulbAdapater::getAllAttributes(), if the snapshot is new enough (maybe 500-1000ms, this is defo a magic number, but its such a clean solution) then use that instead of doing a new fetch

// Intermediate adapter to generalise functions for the rest of the program
// You can change the method implementation to match your bulb API of choice

static const std::string bulbIp = "192.168.1.203";

class SmartBulbAdapter
{
private:
	L530 *core;
	unsigned long prevMillis;

	int freshSnapshotThreshold_ms = 1000; // I LOVE MAGIC NUMBERS!!!!!
	unsigned long attributeSnapshotMillis;
	ColorBulbAttributes attributeSnapshot;

	bool errorCodeToBool(int errorCode)
	{
		return errorCode == 0;
	}

	// Creates new snapshot if newest one is "stale"
	void ensureSnapshotFreshness()
	{
		if (!isSnapshotFresh())
			createSnapshot();
	}

	void createSnapshot()
	{
		attributeSnapshot = core->getAttributes();
		attributeSnapshotMillis = millis();
	}

	bool isSnapshotFresh()
	{
		return (millis() - attributeSnapshotMillis < freshSnapshotThreshold_ms);
	}

public:
	~SmartBulbAdapter()
	{
		delete core;
	}

	// This should be verified before beginning
	static bool isDeviceOn()
	{
		HTTPClient http;
		http.begin(("http://" + bulbIp).c_str());
		int responseCode = http.GET();
		http.end();

		return responseCode == 200;
	}

	void begin()
	{
		prevMillis = millis();
		core = new L530(bulbIp);
	}

	void tick()
	{
		core->incrementCookieAge(millis() - prevMillis);
		prevMillis = millis();
	}

	// 0 for off, 1 for on
	int getPowerState()
	{
		ensureSnapshotFreshness();
		return (int)(attributeSnapshot.powerState);
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
		ensureSnapshotFreshness();
		return attributeSnapshot.brightness;
	}

	bool setBrightness(int brightness)
	{
		return errorCodeToBool(core->setBrightness(brightness));
	}

	int getColorTemperature()
	{
		ensureSnapshotFreshness();
		return attributeSnapshot.colorTemperature;
	}

	bool setColorTemperature(int temperature)
	{
		return errorCodeToBool(core->setColorTemperature(temperature));
	}

	int getHue()
	{
		ensureSnapshotFreshness();
		return attributeSnapshot.hue;
	}

	bool setHue(int hue)
	{
		return errorCodeToBool(core->setHue(hue));
	}

	int getSaturation()
	{
		ensureSnapshotFreshness();
		return attributeSnapshot.saturation;
	}

	bool setSaturation(int saturation)
	{
		return errorCodeToBool(core->setSaturation(saturation));
	}
};

#endif