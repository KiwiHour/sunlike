#pragma once

#include <functional>
#include <nlohmann/json.hpp>
#include <HTTPClient.h>

class GenericDirector
{
public:
	// Returns current hours and minutes (requires internet)
	std::pair<int, int> getHoursMinutes();

	// Returns value from 0.0 to 1.0 based on how far through the light scene is
	virtual float getProgress() = 0;
};