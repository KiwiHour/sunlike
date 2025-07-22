#include "GenericMenuValue.h"

void GenericMenuValue::handleInput(SwitchInput input)
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

		adjustValue(delta);
		circularClamp();
	}
}

std::string GenericMenuValue::getFormattedValue()
{
	return std::to_string(getValue()) + unit;
}

void GenericMenuValue::clamp()
{
	int clampedValue = max(minValue, min(maxValue, getValue()));
	setValue(clampedValue);
}
// Clamps values, but going below or under the limits causes it to loop back
void GenericMenuValue::circularClamp()
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

int GenericMenuValue::getValue()
{
	return internalMenuValue;
}

bool GenericMenuValue::setValue(int value)
{
	internalMenuValue = value;
	modified = true;
	return true;
}

bool GenericMenuValue::adjustValue(int delta)
{
	int current = getValue();
	return setValue(current + delta);
}

void GenericMenuValue::setAndFlushInternalValue()
{
	if (!modified)
		return;

	modified = false;
	state.set(stateName, getValue());
	state.flush(stateName);
}