#pragma once

#include "GenericMenuValue.h"

class TimeMenuValue : public GenericMenuValue
{
public:
	// max value is set to 23 if its for an hour unit, or 59 otherwise.
	TimeMenuValue(const std::string &_stateName, TimeUnit unit)
		: GenericMenuValue(_stateName, 0, unit == TimeUnit::HOUR ? 23 : 59), padSize(2), padChar('0') {}

	TimeMenuValue(TimeUnit unit)
		: GenericMenuValue(0, unit == TimeUnit::HOUR ? 23 : 59), padSize(2), padChar('0') {}

	virtual std::string getFormattedValue();

private:
	size_t padSize;
	char padChar;
};