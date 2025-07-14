#pragma once

#include <Arduino.h>

// Uses the getLocalTime function from ESP library
tm getCurrentTime();
int getCurrentHour();
int getCurrentMinute();