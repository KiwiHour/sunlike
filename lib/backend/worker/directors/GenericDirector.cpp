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

std::optional<int> GenericDirector::getSecondsElapsed()
{
	tm now = getCurrentTime();
	auto [startHour, startMinute] = getStartTime();

	// Used for when start time hasn't been decided yet
	if (startHour == -1 && startMinute == -1)
		return std::nullopt;

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
	std::optional<int> elapsed_s = getSecondsElapsed();

	if (!elapsed_s.has_value())
	{
		logCritical("Director getProgress was called when the elapsed seconds is null (most likely calling DuskfallDirector without setting the start time)");
		return 0;
	}

	// Log on debug level just in case it is a mistake
	if (totalDuration_s <= 0)
		logDebug("The total duration of a director is 0");

	if (elapsed_s.value() > totalDuration_s)
		return 1.0f; // Finished, cap it

	return (float)elapsed_s.value() / totalDuration_s;
}

void GenericDirector::onEnter()
{
	initialBulbState.powerState = state.get(StateName::Bulb::PowerState);
	initialBulbState.brightness = state.get(StateName::Bulb::Brightness);
	initialBulbState.colorTemperature = state.get(StateName::Bulb::ColorTemperature);
	initialBulbState.hue = state.get(StateName::Bulb::Hue);
	initialBulbState.saturation = state.get(StateName::Bulb::Saturation);
}