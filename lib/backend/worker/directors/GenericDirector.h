#pragma once

#include <functional>
#include <nlohmann/json.hpp>
#include <Arduino.h>
#include "core/tapo/tapo-core.cpp"

class GenericDirector
{
public:
	// Uses the getProgress method to produce the current bulb state
	virtual ColorBulbAttributes getBulbState() = 0;

	// Hours and minutes
	virtual pair<int, int> getStartTime() = 0;
	// Hours and minutes
	virtual pair<int, int> getDuration() = 0;

protected:
	// Returns value from 0.0 to 1.0 based on how far through the light scene is
	// Which is then transformed by the specific director to fit how the light's rate of change should be
	// Eg. default is linear, but i can be applied to a sigmoid function to transform it
	// TODO: may need a parameter for pausing
	float getProgress();
};