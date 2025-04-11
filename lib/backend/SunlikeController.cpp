#ifndef SUNLIKE_CONTROLLER
#define SUNLIKE_CONTROLLER

#include "core/SmartBulbAdapter.cpp"

class SunlikeController
{
private:
	SmartBulbAdapter *bulb;

public:
	SunlikeController(SmartBulbAdapter *adapter)
	{
		bulb = adapter;
	}

	void tick(bool erm)
	{
	}
};

#endif