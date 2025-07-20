#include "SunlikeWorker.h"

void SunlikeWorker::tick()
{
	GenericDirector *director = getMostRecentDirector();

	// Nothing to do...
	if (director == nullptr)
		return;

	Serial.println(director->getProgress(), 5);
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