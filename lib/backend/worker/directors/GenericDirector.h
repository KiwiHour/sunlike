#pragma once

#include <functional>
#include <nlohmann/json.hpp>
#include <Arduino.h>
#include <HTTPClient.h>

class GenericDirector
{
public:
	// Returns value from 0.0 to 1.0 based on how far through the light scene is
	virtual float getProgress() = 0;
};