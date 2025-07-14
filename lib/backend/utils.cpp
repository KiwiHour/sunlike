#include "utils.h"

tm getCurrentTime()
{
	struct tm timeInfo;
	if (!getLocalTime(&timeInfo))
	{
		Serial.println("Failed to get time");
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
