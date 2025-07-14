#include "utils.h"

tm getCurrentTime()
{
	tm timeInfo;
	if (!getLocalTime(&timeInfo))
	{
		logCritical("Failed to get time");
		return timeInfo;
	}

	return timeInfo;
}

int getCurrentHour()
{
	return getCurrentTime().tm_hour;
}

int getCurrentMinute()
{
	return getCurrentTime().tm_min;
}
