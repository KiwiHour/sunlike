#pragma once

#include "GenericDirector.h"

class SunriseDirector : public GenericDirector
{
public:
	float getProgress();
	ColorBulbAttributes getBulbState() override;
	TimeHM getStartTime() override;
	TimeHM getDuration() override;
};
