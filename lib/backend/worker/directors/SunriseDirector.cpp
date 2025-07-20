#include "SunriseDirector.h"

std::pair<int, int> SunriseDirector::getStartTime()
{
	return {state.get("sunrise_start_hour"), state.get("sunrise_start_minute")};
}

std::pair<int, int> SunriseDirector::getDuration()
{
	return {state.get("sunrise_duration_hour"), state.get("sunrise_duration_minute")};
}

ColorBulbAttributes SunriseDirector::getBulbState()
{
	return ColorBulbAttributes{};
}

float SunriseDirector::getProgress()
{
	float baseProgress = GenericDirector::getProgress();
	return (2 * baseProgress) / (abs(baseProgress) + 1);
}