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

// TODO:

// Add sunset director logic
// Add duskfall director logic and start duskfall function logic

// Add pausing to the directors
// - Need a current pause start (to know if we are currently paused) and current pause duration (for cancelling purposes) and TOTAL pause duration (a running total of the current pause(s) to allow for multiple pauses)
// - When the director exits, it should reset the current pause start+duration and the total pause duration, even if there wasn't a pause it should still be done because its consistent and more stable
// - Don't allow director to change if currently paused to allow for pausing right before a director change (eg, "oh no, its about to be sunset, i need another hour before that")
// - Adjust current time used in progress calcs by the TOTAL duration (do current elapsed - total duration)
// - If it's currently paused (aka, if current elapsed is in the range of start time and (start time + duration)) then use the start time as the current elapsed for the progress calculations
// - Pause should be a function worded as (Add pause time) to show its adding to any previous pauses
// - I think adjusting wont work, do cancel only (obv only allowed it currently paused)
// - When cancelling, set the current pause duration to (current elapsed - start elapsed), and then lower the TOTAL pause duration accordingly

// Find a way to make brightness changes smoother - saturation -> hue -> brightness seem to be the priority in what causes different levels of 'brightness'
// 		Figuring out how to link up brightness=x1, sat=y1, hue=z1 and brightness=x2, sat=y2, hue=z2 seems to be the answer
//		Ideally I need to find where the "jumpy points" are, and just avoid them if possible, i think this happens when the brightness gets very low and the saturation is at a specific point

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
	configTime(0, daylightOffset, "pool.ntp.org"); // UTC-0
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
	state.addValue(StateName::Bulb::Mode, bind(&SmartBulbAdapter::getMode, &bulb), [](int)
				   { return true; });

	// ######################
	// #####   CONFIG   #####
	// ######################

	// Preference keys are max 15 characters, which is why some of the naming is weird
	std::vector<std::pair<std::string, std::string>>
		stateNamePrefKeyPairs = {
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