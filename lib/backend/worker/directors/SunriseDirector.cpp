#include "SunriseDirector.h"

ColorBulbAttributes SunriseDirector::getBulbState()
{
	ColorBulbAttributes attributes;

	attributes.brightness = (int)std::round(getProgress() * 100.0f);
	attributes.colorTemperature = (int)std::round(getProgress() * 300.0f) + (2850 - 300);

	return attributes;
}

float SunriseDirector::getProgress()
{
	float baseProgress = GenericDirector::getProgress();
	return (2 * baseProgress) / (abs(baseProgress) + 1);
}