#pragma once

#include "GenericDirector.h"

class DuskfallDirector : public GenericDirector
{
public:
	using GenericDirector::GenericDirector;

	float getProgress();
	ColorBulbAttributes getBulbState() override;
	void onExit() override;
	std::string getName() override
	{
		return "duskfall";
	}
};
