#pragma once

#include "GenericMenuValue.h"

class DurationMenuValue : public GenericMenuValue
{
public:
	DurationMenuValue(const std::string &_stateName, TimeUnit _unit, int _minValue, int _maxValue)
		: GenericMenuValue(_stateName, _minValue, _maxValue), unit(_unit) {}

	DurationMenuValue(TimeUnit _unit, int _minValue, int _maxValue)
		: GenericMenuValue(_minValue, _maxValue), unit(_unit) {}

	virtual std::string getFormattedValue();

private:
	TimeUnit unit;
	std::string getStringUnit();
};