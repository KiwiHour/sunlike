#pragma once

#include "GenericDirector.h"

class SunriseDirector : public GenericDirector
{
public:
	using GenericDirector::GenericDirector;

	float getProgress();
	ColorBulbAttributes getBulbState() override;
	std::string getName() override
	{
		return "sunrise";
	}
};
