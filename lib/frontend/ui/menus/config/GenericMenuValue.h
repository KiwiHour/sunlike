#pragma once

#include "ui/Menu.h"
#include "state/StateController.h"

typedef std::function<int()> Getter;
typedef std::function<bool(int)> Setter;

enum class TimeUnit
{
	HOUR,
	MINUTE,
	SECOND,
};

class GenericMenuValue
{
public:
	std::string stateName;
	int internalMenuValue;

	GenericMenuValue(const std::string &_stateName, int _minValue, int _maxValue, const std::string &_unit = "")
		: stateName(_stateName), minValue(_minValue), maxValue(_maxValue), unit(_unit)
	{
		// The constant number here is a scalar for how fast the value changes when holding an input
		// Higher deltaRate means slower increase (stupid I know), so decreasing the numerator in the round function will speed up the rate
		// Clamped between 1 and 6, anything higher than 8 proved to be a bit too slow even for small ranges
		deltaRate = max(1, min(6, (int)round(225.0 / (maxValue - minValue))));

		internalMenuValue = state.get(stateName);
	}

	void handleInput(SwitchInput input);
	virtual std::string getFormattedValue();
	void setAndFlushInternalValue();

	// Clamps values, but going below or under the limits causes it to loop back
	void circularClamp();
	void clamp();

protected:
	int minValue;
	int maxValue;
	bool modified = false;
	std::string unit;

	int deltaRate;
	int holdCount;

	int getValue();
	bool setValue(int value);
	bool adjustValue(int delta);
};