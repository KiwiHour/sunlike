#include "SunlikeWorker.h"

void SunlikeWorker::tick()
{
	GenericDirector *director = getMostRecentDirector();

	// If there are no directors, or in manual override mode, or bulb is off
	// Don't do anything
	if (
		director == nullptr ||
		state.get("is_manual_override") == 1 ||
		state.get("bulb_power_state") == 0)
	{
		return;
	}

	int goalBrightness = director->getBulbState().brightness;

	if (goalBrightness != state.get("bulb_brightness"))
	{
		state.set("bulb_brightness", goalBrightness);
		state.flush("bulb_brightness");
	}
}

GenericDirector *SunlikeWorker::getMostRecentDirector()
{
	GenericDirector *mostRecentDirector = nullptr;
	tm now = getCurrentTime();

	// Aim is to find the director with the lowest time "elapsed"/time since last started
	for (const auto &director : directors)
	{
		if (
			mostRecentDirector == nullptr ||
			director->getSecondsElapsed() < mostRecentDirector->getSecondsElapsed())
		{
			mostRecentDirector = director.get();
		}
	}

	if (mostRecentDirector == nullptr)
		logCritical("No directors");

	return mostRecentDirector;
}