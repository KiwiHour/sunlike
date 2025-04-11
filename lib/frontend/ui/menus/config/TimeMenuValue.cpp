#ifndef TIME_MENU_VALUE
#define TIME_MENU_VALUE

#include "GenericMenuValue.cpp"

class TimeMenuValue : public GenericMenuValue
{
public:
	// max value is set to 23 if its for an hour unit, or 59 otherwise.
	TimeMenuValue(const std::string &_stateName, TimeUnit unit) : GenericMenuValue(_stateName, 0, unit == TimeUnit::HOUR ? 23 : 59)
	{
		padSize = 2;
		padChar = '0';
	}

	virtual string getFormattedValue()
	{
		string str = to_string(getValue());

		// Pad if desired
		if (padSize > 0)
		{
			if (str.length() < padSize)
				str = string(padSize - str.length(), padChar) + str;
		}

		return str;
	}

private:
	size_t padSize;
	char padChar;
};

#endif