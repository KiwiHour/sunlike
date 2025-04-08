#ifndef VALUE_CONTROLLER
#define VALUE_CONTROLLER

#include "../menu.h"
#include "../backend/state_controller.h"

typedef function<int()> Getter;
typedef function<bool(int)> Setter;

class ValueController
{
public:
	ValueController(const std::string &_name, int _minValue, int _maxValue, size_t _padSize = 0, char _padChar = '?')
	{
		name = _name;
		minValue = _minValue;
		maxValue = _maxValue;
		padSize = _padSize;
		padChar = _padChar;

		// The constant number here is a scalar for how fast the value changes when holding an input
		// Higher deltaRate means slower increase (stupid I know), so decreasing the constant number will speed up the rate
		// Clamped between 1 and 6, anything higher than 8 proved to be a bit too slow even for small ranges
		deltaRate = max(1, min(6, (int)round(225.0 / (maxValue - minValue))));
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

			state->adjust(name, delta);
			circularClamp();
		}
	}

	string getFormattedValue()
	{
		string str = to_string(getValue());
		Serial.println(getValue());

		// Pad if desired
		if (padSize > 0)
		{
			if (str.length() < padSize)
				str = string(padSize - str.length(), padChar) + str;
		}

		return str;
	}

	void clamp()
	{
		int clampedValue = max(minValue, min(maxValue, getValue()));
		setValue(clampedValue);
	}
	// Clamps values, but going below or under the limits causes it to loop back
	void circularClamp()
	{
		if (getValue() > maxValue)
		{
			setValue(minValue);
		}
		else if (getValue() < minValue)
		{
			setValue(maxValue);
		}
	}

private:
	std::string name;
	int minValue;
	int maxValue;

	size_t padSize;
	char padChar;

	int deltaRate;
	int holdCount;

	int getValue()
	{
		return state->get(name);
	}

	bool setValue(int value)
	{
		return state->set(name, value);
	}
};

#endif