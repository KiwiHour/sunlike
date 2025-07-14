#pragma once

#include <Arduino.h>

#define DEBUG 1
#define logCritical(message, ...)          \
	Serial.print(F("[CRITICAL] "));        \
	Serial.printf(message, ##__VA_ARGS__); \
	Serial.println();

#if DEBUG == 1
#define logDebug(message, ...)             \
	Serial.print(F("[DEBUG] "));           \
	Serial.printf(message, ##__VA_ARGS__); \
	Serial.println();
#endif

// Config management using preferences
int getConfig(std::string key);
bool setConfig(std::string key, int value);
bool isPreferenceKeyTooLong(std::string key);
std::pair<std::function<int()>, std::function<bool(int)>> createConfigGetterAndSetter(std::string key);

// Uses the getLocalTime function from ESP library
tm getCurrentTime();
int getCurrentHour();
int getCurrentMinute();