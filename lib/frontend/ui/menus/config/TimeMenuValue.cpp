#include "TimeMenuValue.h"

std::string TimeMenuValue::getFormattedValue()
{
	std::string str = std::to_string(getValue());

	// Pad if desired
	if (padSize > 0)
	{
		if (str.length() < padSize)
			str = std::string(padSize - str.length(), padChar) + str;
	}

	return str;
}
