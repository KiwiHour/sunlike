#pragma once

#include <Arduino.h>
#include "tapo/tapo-core.cpp"

// Intermediate adapter to generalise functions for the rest of the program
// You can change the method implementation to match your bulb API of choice

static const std::string bulbIp = "192.168.1.203";

class SmartBulbAdapter
{
private:
	L530 *core;
	unsigned long prevMillis;

	int freshSnapshotThreshold_ms = 1000;
	unsigned long attributeSnapshotMillis;
	ColorBulbAttributes attributeSnapshot;

	bool errorCodeToBool(int errorCode);

	// Creates new snapshot if newest one is "stale"
	void ensureSnapshotFreshness();
	void createSnapshot();
	bool isSnapshotFresh();

public:
	~SmartBulbAdapter()
	{
		delete core;
	}

	// This should be verified before beginning
	static bool isDeviceOn();

	void begin();
	void tick();

	// 0 for off, 1 for on
	int getPowerState();
	bool setPowerState(int state);

	int getBrightness();
	bool setBrightness(int brightness);

	int getColorTemperature();
	bool setColorTemperature(int temperature);

	int getHue();
	bool setHue(int hue);

	int getSaturation();
	bool setSaturation(int saturation);
};