#pragma once

#include <Arduino.h>

#define DEBUG 1
#define logCritical(message)        \
	Serial.print(F("[CRITICAL] ")); \
	Serial.println(message);
#define logfCritical(message, var)  \
	Serial.print(F("[CRITICAL] ")); \
	Serial.printf(message, var);

#if DEBUG == 1
#define logDebug(message)        \
	Serial.print(F("[DEBUG] ")); \
	Serial.println(message);
#define logfDebug(message, var)  \
	Serial.print(F("[DEBUG] ")); \
	Serial.printf(message, var);
#endif

// Uses the getLocalTime function from ESP library
tm getCurrentTime();
int getCurrentHour();
int getCurrentMinute();