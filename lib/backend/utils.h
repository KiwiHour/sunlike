#pragma once

#include <Arduino.h>

#define DEBUG 1
#define logCritical(message, ...)              \
	do                                         \
	{                                          \
		Serial.print(F("[CRITICAL] "));        \
		Serial.printf(message, ##__VA_ARGS__); \
		Serial.println();                      \
	} while (0)

#if DEBUG == 1
#define logDebug(message, ...)                 \
	do                                         \
	{                                          \
		Serial.print(F("[DEBUG] "));           \
		Serial.printf(message, ##__VA_ARGS__); \
		Serial.println();                      \
	} while (0)
#endif

// Config management using preferences
int getConfig(const std::string &key);
bool setConfig(const std::string &key, int value);
bool isPreferenceKeyTooLong(const std::string &key);
std::pair<std::function<int()>, std::function<bool(int)>> createConfigGetterAndSetter(const std::string &key);

// Uses the getLocalTime function from ESP library
tm getCurrentTime();
int getCurrentHour();
int getCurrentMinute();