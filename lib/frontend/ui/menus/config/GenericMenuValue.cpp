#ifndef GENERIC_MENU_VALUE
#define GENERIC_MENU_VALUE

#include "../frontend/ui/Menu.h"
#include "../backend/state/StateController.h"

typedef function<int()> Getter;
typedef function<bool(int)> Setter;

enum class TimeUnit
{
	HOUR,
	MINUTE,
	SECOND,
};

class GenericMenuValue
{
public:
	std::string stateName;

	GenericMenuValue(const std::string &_stateName, int _minValue, int _maxValue, string _unit = "")
	{
		stateName = _stateName;
		minValue = _minValue;
		maxValue = _maxValue;
		unit = _unit;

		// The constant number here is a scalar for how fast the value changes when holding an input
		// Higher deltaRate means slower increase (stupid I know), so decreasing the numerator in the round function will speed up the rate
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

			state->adjust(stateName, delta);
			circularClamp();
		}
	}

	virtual string getFormattedValue()
	{
		return to_string(getValue()) + unit;
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

protected:
	int minValue;
	int maxValue;
	string unit;

	int deltaRate;
	int holdCount;

	int getValue()
	{
		return state->get(stateName);
	}

	bool setValue(int value)
	{
		return state->set(stateName, value);
	}
};

#endif