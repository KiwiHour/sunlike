#ifndef SUNLIKE_CONTROLLER
#define SUNLIKE_CONTROLLER

#include "core/SmartBulbAdapter.cpp"
#include "state/StateController.h"
#include "directors/GenericDirector.h"

class SunlikeWorker
{
private:
	SmartBulbAdapter *bulb;

public:
	SunlikeWorker(SmartBulbAdapter *adapter) : bulb(adapter) {}

	void tick();
};

#endif