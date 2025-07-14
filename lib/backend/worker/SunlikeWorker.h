#pragma once

#include "core/SmartBulbAdapter.h"
#include "state/StateController.h"
#include "worker/directors/GenericDirector.h"

class SunlikeWorker
{
private:
	SmartBulbAdapter *bulb;

public:
	SunlikeWorker(SmartBulbAdapter *adapter) : bulb(adapter) {}

	void tick();
};