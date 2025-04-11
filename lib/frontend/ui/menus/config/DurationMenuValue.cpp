#ifndef DURATION_MENU_VALUE
#define DURATION_MENU_VALUE

#include "GenericMenuValue.cpp"

class DurationMenuValue : public GenericMenuValue
{
public:
	DurationMenuValue(const std::string &_stateName, TimeUnit _unit, int _minValue, int _maxValue) : GenericMenuValue(_stateName, _minValue, _maxValue)
	{
		unit = _unit;
	}

	virtual string getFormattedValue()
	{
		string str = to_string(getValue());
		return str + getStringUnit();
	}

private:
	TimeUnit unit;

	string getStringUnit()
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
};

#endif