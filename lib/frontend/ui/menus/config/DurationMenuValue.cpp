#include "DurationMenuValue.h"

std::string DurationMenuValue::getFormattedValue()
{
	std::string str = std::to_string(getValue());
	return str + getStringUnit();
}

std::string DurationMenuValue::getStringUnit()
{
	switch (unit)
	{
	case TimeUnit::HOUR:
		return "h";
		break;
	case TimeUnit::MINUTE:
		return "m";
		break;
	case TimeUnit::SECOND:
		return "s";
		break;
	default:
		return "?";
		break;
	}
}