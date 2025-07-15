#pragma once

#include "GenericDirector.h"

class SunriseDirector : public GenericDirector
{
public:
	float getProgress();
	ColorBulbAttributes getBulbState() override;
	std::pair<int, int> getStartTime() override;
	std::pair<int, int> getDuration() override;
};
