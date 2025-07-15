#include "GenericDirector.h"
#include "utils.h"

float GenericDirector::getProgress()
{
	auto [startHour, startMinute] = getStartTime();
	auto [durationHour, durationMinute] = getDuration();
	tm now = getCurrentTime();

	int totalDuration_s = (durationHour * 60 * 60) + (durationMinute * 60);
	int elapsed_s = ((now.tm_hour - startHour) * 60 * 60) + ((now.tm_min - startMinute) * 60) + now.tm_sec;

	// Log on debug level just in case it is a mistake
	if (totalDuration_s <= 0)
		logDebug("Total duration of a director is 0");

	if (elapsed_s > totalDuration_s)
		return 1.0f; // Finished
	if (elapsed_s < 0)
		return 0.0f; // Hasn't started

	return elapsed_s / totalDuration_s;
}