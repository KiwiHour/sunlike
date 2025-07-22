#include "SunriseDirector.h"

ColorBulbAttributes SunriseDirector::getBulbState()
{
	return ColorBulbAttributes{};
}

float SunriseDirector::getProgress()
{
	float baseProgress = GenericDirector::getProgress();
	return (2 * baseProgress) / (abs(baseProgress) + 1);
}