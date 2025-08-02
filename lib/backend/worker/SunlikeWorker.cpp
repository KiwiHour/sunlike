#include "SunlikeWorker.h"

void SunlikeWorker::maybeSetAndFlush(const std::string &stateName, std::optional<int> value)
{
	BulbMode bulbMode = static_cast<BulbMode>(state.get(StateName::Bulb::Mode));
	bool differingBulbMode = ((stateName == StateName::Bulb::ColorTemperature && bulbMode != BulbMode::ColorTemperature) ||
							  ((stateName == StateName::Bulb::Hue || stateName == StateName::Bulb::Saturation) && bulbMode != BulbMode::HueSaturation));

	// If there is a value, and that value is different to the current one (aka, needs to change), or if the bulb mode differs (eg. in color temp mode when it should be in huesat)
	if (value.has_value() && (differingBulbMode || value.value() != state.get(stateName)))
	{
		state.setAndFlush(stateName, value.value());
	}
}

void SunlikeWorker::tick()
{
	if (currentDirector == nullptr)
	{
		logCritical("Current director somehow nullptr");
		return;
	}

	if (
		state.get(StateName::Misc::IsManualOverride) == 1 ||
		state.get(StateName::Bulb::PowerState) == 0)
	{
		return;
	}

	// Guard clauses passed, begin logic

	GenericDirector *newDirectorCandidate = getNewDirector();

	// If the new director is different, then we need to transition to the new one and trigger its onEnter function
	if (currentDirector != newDirectorCandidate)
	{
		currentDirector->onExit();
		currentDirector = newDirectorCandidate;
		currentDirector->onEnter();
		logDebug("Switched director to '%s'", currentDirector->getName().c_str());
	}

	ColorBulbAttributes newBulbState = currentDirector->getBulbState();

	// Set all the values that have been explicitly set in the state
	maybeSetAndFlush(StateName::Bulb::PowerState, newBulbState.powerState);
	maybeSetAndFlush(StateName::Bulb::Brightness, newBulbState.brightness);
	maybeSetAndFlush(StateName::Bulb::ColorTemperature, newBulbState.colorTemperature);
	maybeSetAndFlush(StateName::Bulb::Hue, newBulbState.hue);
	maybeSetAndFlush(StateName::Bulb::Saturation, newBulbState.saturation);
}

GenericDirector *SunlikeWorker::getNewDirector()
{

	GenericDirector *candidateDirector = currentDirector;
	int candidateSecondsElapsed = candidateDirector->getSecondsElapsed().value();

	// Aim is to find the director with the lowest time "elapsed"/time since last started
	for (const auto &director : directors)
	{
		// Don't compare against yourself
		if (director.get() == candidateDirector)
			continue;

		std::optional<int> secondsElapsed = director->getSecondsElapsed();

		// Check both elapsed seconds are valid (have a start time, then check for smallest one)
		if ((secondsElapsed.has_value()) && secondsElapsed.value() < candidateSecondsElapsed)
		{
			candidateSecondsElapsed = secondsElapsed.value(); // Take the director we are looking at's seconds elapsed as the new one
			candidateDirector = director.get();
		}
	}

	return candidateDirector;
}