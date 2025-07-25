#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <time.h>

#include "utils.h"
#include "network-credentials.h"
#include "InputHandler.h"
#include "ui/UI.h"
#include "worker/SunlikeWorker.h"
#include "state/StateController.h"

UI ui;
InputHandler inputHandler;
SmartBulbAdapter bulb;
SunlikeWorker sunlike;

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
			logCritical("Failed to connect");
			return false;
		}
		delay(250);
	}

	Serial.println("Connected");

	return true;
}

void waitUntilBulbOn()
{
	while (true)
	{
		if (SmartBulbAdapter::isDeviceOn())
		{
			// Leave while loop, and continue rest of the setup
			logDebug("Verified bulb has power");
			break;
		}

		logCritical("Couldn't connect to bulb, is it turned off at the switch?");
		delay(10000); // 10s
	}
}

void setupTime()
{
	int daylightOffset = (bool)(state.get(StateName::Misc::IsDaylightSavingTime)) ? 3600 : 0;
	configTime(0, daylightOffset, "pool.ntp.org");
	logDebug("Time configured");

	tm now = getCurrentTime();
	char buffer[64];
	strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", &now);
	std::string formattedTime = buffer;
	logDebug(formattedTime.c_str());
}

void buildState()
{
	// ######################
	// #####    BULB    #####
	// ######################

	// TODO: switch states to nested enums like (State::Bulb::PowerState)

	state.addValue(StateName::Bulb::PowerState, bind(&SmartBulbAdapter::getPowerState, &bulb), bind(&SmartBulbAdapter::setPowerState, &bulb, placeholders::_1));
	state.addValue(StateName::Bulb::Brightness, bind(&SmartBulbAdapter::getBrightness, &bulb), bind(&SmartBulbAdapter::setBrightness, &bulb, placeholders::_1));
	state.addValue(StateName::Bulb::ColorTemperature, bind(&SmartBulbAdapter::getColorTemperature, &bulb), bind(&SmartBulbAdapter::setColorTemperature, &bulb, placeholders::_1));
	state.addValue(StateName::Bulb::Hue, bind(&SmartBulbAdapter::getHue, &bulb), bind(&SmartBulbAdapter::setHue, &bulb, placeholders::_1));
	state.addValue(StateName::Bulb::Saturation, bind(&SmartBulbAdapter::getSaturation, &bulb), bind(&SmartBulbAdapter::setSaturation, &bulb, placeholders::_1));

	// ######################
	// #####   CONFIG   #####
	// ######################

	// Preference keys are max 15 characters, which is why some of the naming is weird
	std::vector<std::pair<std::string, std::string>> stateNamePrefKeyPairs = {
		// Sunrise
		{StateName::Sunrise::StartHour, "rise_hour"},
		{StateName::Sunrise::StartMinute, "rise_minute"},
		{StateName::Sunrise::DurationHour, "rise_dur_min"},
		{StateName::Sunrise::DurationMinute, "rise_dur"},

		// Sunset
		{StateName::Sunset::StartHour, "set_hour"},
		{StateName::Sunset::StartMinute, "set_minute"},
		{StateName::Sunset::DurationHour, "set_dur_hour"},
		{StateName::Sunset::DurationMinute, "set_dur_min"},

		// Duskfall
		{StateName::Duskfall::StartHour, "dusk_hour"},
		{StateName::Duskfall::StartMinute, "dusk_min"},
		{StateName::Duskfall::DurationHour, "dusk_dur_hour"},
		{StateName::Duskfall::DurationMinute, "dusk_dur_min"},

		// Misc
		// because I'm lazy, you'll need to restart after updating the daylight offset, real-time updates would be a pain
		{StateName::Misc::IsDaylightSavingTime, "is_dst"},
		{StateName::Misc::IsManualOverride, "is_manual_ovrd"},
	};

	for (auto [stateName, prefKey] : stateNamePrefKeyPairs)
	{
		state.addValue(stateName, createConfigGetterAndSetter(prefKey));
	}

	// Setup unique situtation of the duskfall start time
	// If it's first time, the duskfall start time is 0,0 which will cause problems
	// For first time, set them as -1,-1 (not usable until user triggers duskfall)
	if (!isPreferenceKey("dusk_hour") || !isPreferenceKey("dusk_min"))
	{
		state.setAndFlush(StateName::Duskfall::StartHour, -1);
		state.setAndFlush(StateName::Duskfall::StartMinute, -1);
	}

	// Fetch all the values to make the state up to date
	state.fetch();
	logDebug("State built");
}

void setup()
{
	Serial.begin(115200);
	Serial.setDebugOutput((bool)DEBUG);
	delay(2000);

	if (!connectOverWiFi())
	{
		// Surely a restart will fix it
		ESP.restart();
		return;
	}
	waitUntilBulbOn();

	bulb.begin();
	logDebug("Bulb begun");
	inputHandler.begin();
	logDebug("Input handler begun");

	// Let bulb and input handler settle
	delay(1000);

	buildState();
	setupTime();

	ui.begin();
	logDebug("Setup completed");
}

unsigned long previousMillis = millis();
void loop()
{
	SwitchInput input = inputHandler.getInput();

	ui.handleInput(input);
	ui.tick();

	if (millis() - previousMillis > 1000)
	{
		previousMillis = millis();
		sunlike.tick();
	}

	delay(10);
}