#pragma once

#include <memory>
#include "utils.h"
#include "state/StateController.h"
#include "core/SmartBulbAdapter.h"
#include "worker/directors/GenericDirector.h"
#include "worker/directors/SunriseDirector.h"
#include "worker/directors/SunsetDirector.h"
#include "worker/directors/DuskfallDirector.h"

class SunlikeWorker
{
private:
	GenericDirector *currentDirector = nullptr;
	std::vector<std::unique_ptr<GenericDirector>> directors;

	// Gets most recently started director (even if it has ended), this is used to determine the current bulb state
	GenericDirector *getNewDirector();
	// Only sets and flushes if value is not nullopt
	void maybeSetAndFlush(const std::string &stateName, std::optional<int> value);

public:
	SunlikeWorker()
	{
		directors.reserve(3);
		directors.push_back(std::make_unique<SunriseDirector>("sunrise"));
		directors.push_back(std::make_unique<SunsetDirector>("sunset"));
		directors.push_back(std::make_unique<DuskfallDirector>("duskfall"));

		currentDirector = directors[0].get(); // Start at the first one, correct director gets decided in the tick
	}

	void tick();
};