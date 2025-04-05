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

		// The constant number here is a scalar for how fast the value changes when holding an input
		// Higher deltaRate means slower increase (stupid I know), so decreasing the constant number will speed up the rate
		// Clamped between 1 and 8, anything higher than 8 proved to be a bit too slow even for small ranges
		deltaRate = max(1, min(8, (int)round(225.0 / (maxValue - minValue))));
	}

	void handleInput(SwitchInput input)
	{
		if (input == SwitchInput::UP || input == SwitchInput::DOWN || input == SwitchInput::HOLDING_UP || input == SwitchInput::HOLDING_DOWN)
		{
			// Inc hold count if holding, just pressed then reset to 0
			if (input == SwitchInput::HOLDING_UP || input == SwitchInput::HOLDING_DOWN)
				holdCount++;
			else
				holdCount = 0;

			// Only change internal value when hold count is multiple of the delta rate
			if (holdCount % deltaRate != 0)
				return;

			// Add delay for holding button to affect value
			if (holdCount != 0 && holdCount < 15)
				return;

			int delta = (input == SwitchInput::UP || input == SwitchInput::HOLDING_UP) ? 1 : -1;

			adjust(delta);
			circularClamp();
		}
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

	int deltaRate;
	int holdCount;
};

#endif