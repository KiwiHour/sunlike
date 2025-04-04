#ifndef VALUE_CONTROLLER
#define VALUE_CONTROLLER

#include "menu.h"

typedef function<int()> Getter;
typedef function<bool(int)> Setter;

class ValueController
{
public:
	ValueController(Getter _getter, Setter _setter, int _minValue, int _maxValue, size_t _padSize = 0, char _padChar = '?')
	{
		getter = _getter;
		setter = _setter;
		minValue = _minValue;
		maxValue = _maxValue;
		padSize = _padSize;
		padChar = _padChar;
	}

	string getFormattedValue()
	{
		string str = to_string(value);

		// Pad if desired
		if (padSize > 0)
		{
			if (str.length() < padSize)
				str = string(padSize - str.length(), padChar) + str;
		}

		return str;
	}

	void set(int _value)
	{
		value = _value;
	}
	int get()
	{
		return value;
	}
	void adjust(int delta)
	{
		set(get() + delta);
	}

	void clamp()
	{
		value = max(minValue, min(maxValue, value));
	}
	// Clamps values, but going below or under the limits causes it to loop back
	void circularClamp()
	{
		if (value > maxValue)
			value = minValue;
		else if (value < minValue)
		{
			value = maxValue;
		}
	}

	// Updates the internal value with the given getter
	void fetch()
	{
		value = getter();
	}
	// Uses the given setter with the internal value
	void flush()
	{
		setter(value);
	}

private:
	int value;
	Getter getter;
	Setter setter;
	int minValue;
	int maxValue;

	size_t padSize;
	char padChar;
};

#endif