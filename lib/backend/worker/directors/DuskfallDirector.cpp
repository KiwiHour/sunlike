#include "DuskfallDirector.h"

void DuskfallDirector::onExit()
{
	// When duskfall finishes (aka moving onto sunrise) reset the time for the next duskfall
	state.set(StateName::Duskfall::StartHour, -1);
	state.set(StateName::Duskfall::StartMinute, -1);
	state.flush(StateName::Duskfall::StartHour);
	state.flush(StateName::Duskfall::StartMinute);
}

float DuskfallDirector::getProgress() { return 0.0f; }
ColorBulbAttributes DuskfallDirector::getBulbState() { return ColorBulbAttributes(); }