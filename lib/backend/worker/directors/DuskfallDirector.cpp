#include "DuskfallDirector.h"

void DuskfallDirector::onExit()
{
	GenericDirector::onExit();
	// When duskfall finishes (aka moving onto sunrise) reset the time for the next duskfall
	state.setAndFlush(StateName::Duskfall::StartHour, -1);
	state.setAndFlush(StateName::Duskfall::StartMinute, -1);
}

float DuskfallDirector::getProgress() { return 0.0f; }
ColorBulbAttributes DuskfallDirector::getBulbState() { return ColorBulbAttributes(); }