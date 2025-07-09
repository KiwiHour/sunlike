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

	Serial.println();

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

	// Preference keys are max 15 characters, which is why some of the naming is weird
	state->addValue("bulb_power_state", bind(&SmartBulbAdapter::getPowerState, bulb), bind(&SmartBulbAdapter::setPowerState, bulb, placeholders::_1));
	state->addValue("bulb_brightness", bind(&SmartBulbAdapter::getBrightness, bulb), bind(&SmartBulbAdapter::setBrightness, bulb, placeholders::_1));
	state->addValue("bulb_color_temperature", bind(&SmartBulbAdapter::getColorTemperature, bulb), bind(&SmartBulbAdapter::setColorTemperature, bulb, placeholders::_1));
	state->addValue("bulb_hue", bind(&SmartBulbAdapter::getHue, bulb), bind(&SmartBulbAdapter::setHue, bulb, placeholders::_1));
	state->addValue("bulb_saturation", bind(&SmartBulbAdapter::getSaturation, bulb), bind(&SmartBulbAdapter::setSaturation, bulb, placeholders::_1));

	// ######################
	// #####   CONFIG   #####
	// ######################

	// Sunrise
	state->addValue("sunrise_start_hour", createConfigGetterAndSetter("rise_hour"), true);
	state->addValue("sunrise_start_minute", createConfigGetterAndSetter("rise_minute"), true);
	state->addValue("sunrise_duration_hour", createConfigGetterAndSetter("rise_dur_min"), true);
	state->addValue("sunrise_duration_minute", createConfigGetterAndSetter("rise_dur"), true);

	// Sunset
	state->addValue("sunset_start_hour", createConfigGetterAndSetter("set_hour"), true);
	state->addValue("sunset_start_minute", createConfigGetterAndSetter("set_minute"), true);
	state->addValue("sunset_duration_hour", createConfigGetterAndSetter("set_dur_hour"), true);
	state->addValue("sunset_duration_minute", createConfigGetterAndSetter("set_dur_min"), true);

	// Duskfall
	state->addValue("duskfall_duration_hour", createConfigGetterAndSetter("dusk_dur_hour"), true);
	state->addValue("duskfall_duration_minute", createConfigGetterAndSetter("dusk_dur_min"), true);

	// Misc.
	// because I'm lazy, you'll need to restart after updating the daylight offset, real-time updates would be a pain
	state->addValue("is_daylight_saving_time", createConfigGetterAndSetter("is_dst"), true);
}

void setup()
{
	Serial.begin(115200);
	delay(2000);

	connectOverWiFi();

	ui = new UI();
	bulb = new SmartBulbAdapter();
	sunlike = new SunlikeWorker(bulb);

	buildState();
	setupTime();

	bulb->begin();
	inputHandler.begin();
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