#pragma once

#include "state/StateController.h"
#include "core/SmartBulbAdapter.h"
#include "state/StateController.h"
#include "worker/directors/GenericDirector.h"

class SunlikeWorker
{
private:
	// Gets most recently started director (even if it has ended), this is used to determine the current bulb state
	GenericDirector getMostRecentDirector();

public:
	void tick();
};