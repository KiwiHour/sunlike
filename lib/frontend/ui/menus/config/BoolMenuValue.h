#pragma once

#include <string>
#include "GenericMenuValue.h"

class BoolMenuValue : public GenericMenuValue
{
public:
	BoolMenuValue(const std::string &_stateName, std::string _trueString, std::string _falseString)
		: GenericMenuValue(_stateName, 0, 1), trueString(_trueString), falseString(_falseString) {}

	virtual std::string getFormattedValue();

private:
	std::string trueString;
	std::string falseString;
};