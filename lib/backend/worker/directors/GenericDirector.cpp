#include "GenericDirector.h"
#include "utils.h"

TimeHM GenericDirector::getStartTime()
{
	return {
		state.get(stateTimePrefix + "_start_hour"),
		state.get(stateTimePrefix + "_start_minute"),
	};
}

TimeHM GenericDirector::getDuration()
{
	return {
		state.get(stateTimePrefix + "_duration_hour"),
		state.get(stateTimePrefix + "_duration_minute"),
	};
}

int GenericDirector::getSecondsElapsed()
{
	tm now = getCurrentTime();
	auto [startHour, startMinute] = getStartTime();

	int now_s = (now.tm_hour * 60 * 60) + (now.tm_min * 60) + now.tm_sec;
	int start_s = (startHour * 60 * 60) + (startMinute * 60);

	const int daySeconds = 24 * 60 * 60;
	return (now_s - start_s + daySeconds) % daySeconds;
}

float GenericDirector::getProgress()
{
	auto [startHour, startMinute] = getStartTime();
	auto [durationHour, durationMinute] = getDuration();
	tm now = getCurrentTime();

	int totalDuration_s = (durationHour * 60 * 60) + (durationMinute * 60);
	int elapsed_s = getSecondsElapsed();

	Serial.printf("start %d %d, dur %d %d, now %d %d, totaldur %d, elapsed %d\n", startHour, startMinute, durationHour, durationMinute, now.tm_hour, now.tm_min, totalDuration_s, elapsed_s);

	// Log on debug level just in case it is a mistake
	if (totalDuration_s <= 0)
		logDebug("The total duration of a director is 0");

	if (elapsed_s > totalDuration_s)
		return 1.0f; // Finished, cap it

	return (float)elapsed_s / totalDuration_s;
}