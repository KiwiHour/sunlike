#include "SunlikeWorker.h"

void SunlikeWorker::tick()
{
	GenericDirector *director = getMostRecentDirector();

	// Nothing to do...
	if (director == nullptr)
		return;

	if (state.get("is_manual_override") == 1)
		return;

	int goalBrightness = director->getBulbState().brightness;

	if (goalBrightness != state.get("bulb_brightness"))
	{
		state.set("bulb_brightness", goalBrightness);

		// Only flush if bulb already on (changing state automatically turns bulb on)
		if (state.get("bulb_power_state") == 1)
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