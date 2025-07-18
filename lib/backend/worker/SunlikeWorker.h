#pragma once

#include "utils.h"
#include "state/StateController.h"
#include "core/SmartBulbAdapter.h"
#include "state/StateController.h"
#include "worker/directors/GenericDirector.h"
#include "worker/directors/SunriseDirector.h"

class SunlikeWorker
{
private:
	std::vector<GenericDirector *> directors;

	// Gets most recently started director (even if it has ended), this is used to determine the current bulb state
	GenericDirector *getMostRecentDirector();

public:
	SunlikeWorker()
	{
		directors.reserve(3);
		directors.push_back(new SunriseDirector());
		// TODO;
		// directors.push_back(new SunsetDirector());
		// directors.push_back(new DuskfallDirector());
	}

	void tick();
};