#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <time.h>
#include <Preferences.h>

#include "../include/network-credentials.h"
#include "../frontend/InputHandler.cpp"
#include "../frontend/ui/UI.cpp"
#include "../backend/worker/SunlikeWorker.cpp"
#include "../backend/state/StateController.h"

// TODO:
// remove namespace std from everwhere
// convert "#ifndef" to "#pragma once"

using namespace std;

UI *ui;
Preferences prefs;
InputHandler inputHandler;
SmartBulbAdapter *bulb;
SunlikeWorker *sunlike;

bool connectOverWiFi()
{
	WiFi.begin(NETWORK_SSID, NETWORK_KEY);
	Serial.printf("Connecting to %s", NETWORK_SSID);

	int tries = 0;
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		if (tries > 50)
		{
			Serial.println("\nFailed to connect");
			return false;
		}
		delay(250);
	}

	Serial.println("Connected");

	return true;
}

bool isPreferenceKeyTooLong(string key)
{
	if (key.length() > 15)
	{
		Serial.printf("Preference key '%s' is longer than 15 characters, which isn't allowed\n", key.c_str());
		return true;
	}
	return false;
}

int getConfig(string key)
{
	if (isPreferenceKeyTooLong(key))
	{
		Serial.printf("Config key '%s' too long", key);
		return -1;
	}
	prefs.begin("sunlike_config");
	int res = prefs.getInt(key.c_str());
	prefs.end();

	return res;
}
bool setConfig(string key, int v)
{
	if (isPreferenceKeyTooLong(key))
	{
		Serial.printf("Config key '%s' too long", key);
		return false;
	}
	prefs.begin("sunlike_config");
	bool success = prefs.putInt(key.c_str(), v);
	prefs.end();

	return success;
}

pair<function<int()>, function<bool(int)>> createConfigGetterAndSetter(string key)
{
	return {
		bind(getConfig, key),
		bind(setConfig, key, placeholders::_1),
	};
}

void setupTime()
{
	int daylightOffset = (bool)(state->get("is_daylight_saving_time")) ? 3600 : 0;
	configTime(0, daylightOffset, "pool.ntp.org");

	struct tm timeInfo;
	if (!getLocalTime(&timeInfo))
	{
		Serial.println("Failed to obtain time");
		return;
	}
	Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
}

void buildState()
{
	// ######################
	// #####    BULB    #####
	// ######################

	state->addValue("bulb_power_state", bind(&SmartBulbAdapter::getPowerState, bulb), bind(&SmartBulbAdapter::setPowerState, bulb, placeholders::_1));
	state->addValue("bulb_brightness", bind(&SmartBulbAdapter::getBrightness, bulb), bind(&SmartBulbAdapter::setBrightness, bulb, placeholders::_1));
	state->addValue("bulb_color_temperature", bind(&SmartBulbAdapter::getColorTemperature, bulb), bind(&SmartBulbAdapter::setColorTemperature, bulb, placeholders::_1));
	state->addValue("bulb_hue", bind(&SmartBulbAdapter::getHue, bulb), bind(&SmartBulbAdapter::setHue, bulb, placeholders::_1));
	state->addValue("bulb_saturation", bind(&SmartBulbAdapter::getSaturation, bulb), bind(&SmartBulbAdapter::setSaturation, bulb, placeholders::_1));

	// ######################
	// #####   CONFIG   #####
	// ######################

	// Preference keys are max 15 characters, which is why some of the naming is weird
	std::vector<std::pair<std::string, std::string>> stateNamePrefKeyPairs = {
		// Sunrise
		{"sunrise_start_hour", "rise_hour"},
		{"sunrise_start_minute", "rise_minute"},
		{"sunrise_duration_hour", "rise_dur_min"},
		{"sunrise_duration_minute", "rise_dur"},

		// Sunset
		{"sunset_start_hour", "set_hour"},
		{"sunset_start_minute", "set_minute"},
		{"sunset_duration_hour", "set_dur_hour"},
		{"sunset_duration_minute", "set_dur_min"},

		// Duskfall
		{"duskfall_duration_hour", "dusk_dur_hour"},
		{"duskfall_duration_minute", "dusk_dur_min"},

		// Misc
		// because I'm lazy, you'll need to restart after updating the daylight offset, real-time updates would be a pain
		{"is_daylight_saving_time", "is_dst"},
	};

	for (auto [stateName, prefKey] : stateNamePrefKeyPairs)
	{
		state->addValue(stateName, createConfigGetterAndSetter(prefKey));
	}

	// Fetch all the values to make the state up to date
	state->fetch();
}

void setup()
{
	Serial.begin(115200);
	delay(2000);

	connectOverWiFi();

	while (true)
	{
		if (SmartBulbAdapter::isDeviceOn())
		{
			// Leave while loop, and continue rest of the setup
			Serial.println("Verified bulb is on");
			break;
		}

		Serial.println("Couldn't connect to bulb, is it turned off at the switch?");
		delay(10000); // 10s
	}

	ui = new UI();
	bulb = new SmartBulbAdapter();
	sunlike = new SunlikeWorker(bulb);

	bulb->begin();
	inputHandler.begin();

	// Let bulb and input handler settle
	delay(1000);

	buildState();
	setupTime();

	ui->begin();
}

void loop()
{
	SwitchInput input = inputHandler.getInput();

	ui->handleInput(input);
	ui->tick();

	// sunlike->tick();

	delay(10);
}