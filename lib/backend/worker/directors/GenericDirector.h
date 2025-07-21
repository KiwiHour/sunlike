#pragma once

#include <functional>
#include <nlohmann/json.hpp>
#include <Arduino.h>
#include "core/tapo/tapo-core.h"
#include "state/StateController.h"

struct TimeHM
{
	int hour, minute;

	TimeHM(int _hour, int _minute)
		: hour(_hour), minute(_minute) {}
};

class GenericDirector
{
public:
	// Uses the getProgress method to produce the current bulb state
	virtual ColorBulbAttributes getBulbState() = 0;
	// Gets seconds since the director started (ignoring duration/end times)
	int getSecondsElapsed();

	// Hours and minutes
	virtual TimeHM getStartTime() = 0;

	// Hours and minutes
	virtual TimeHM getDuration() = 0;

	// Returns value from 0.0 to 1.0 based on how far through the light scene is
	// Which is then transformed by the specific director to fit how the light's rate of change should be
	// Eg. default is linear, but i can be applied to a sigmoid function to transform it
	// TODO: may need a parameter for pausing
	virtual float getProgress();
};