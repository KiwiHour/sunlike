#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <nlohmann/json.hpp>
#include "tapo/tapo-core.h"

enum class BulbMode
{
	ColorTemperature,
	HueSaturation,
	None
};

// Intermediate adapter to generalise functions for the rest of the program
// You can change the method implementation to match your bulb API of choice

static const std::string bulbIp = "192.168.1.203";

class SmartBulbAdapter
{
private:
	L530 *core = nullptr; // Using pointer because the initialisation of the core needs to be delayed
	BulbMode currentMode = BulbMode::None;
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

	// <!!!>
	// Getters and setters should never be used
	// Only ever through the StateController

	int getMode();
	bool batchSet(ColorBulbAttributes attributes);

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