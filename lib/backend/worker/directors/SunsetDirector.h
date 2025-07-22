#pragma once

#include "GenericDirector.h"

class SunsetDirector : public GenericDirector
{
public:
	using GenericDirector::GenericDirector;

	float getProgress();
	ColorBulbAttributes getBulbState() override;
	std::string getName() override
	{
		return "sunset";
	}
};
