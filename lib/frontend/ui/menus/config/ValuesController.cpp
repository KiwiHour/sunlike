#include "ValuesController.h"

void ValuesController::handleInput(SwitchInput input)
{
	if (input == SwitchInput::PUSH)
	{
		// If none selected, select first one. Else, unselect
		if (index == -1)
			index = 0;
		else
		{
			for (const auto value : values)
			{
				value->setAndFlushInternalValue();
			}

			index = -1;
		}
	}
	if (input == SwitchInput::LEFT || input == SwitchInput::RIGHT)
	{
		// Increase if left, decrease if right
		int delta = (input == SwitchInput::LEFT) ? -1 : 1;

		index += delta;
		clampIndex();
	}

	// Handle how the value changes
	if (values.size() > 0)
		values[index]->handleInput(input);
	else
		logDebug("Values controller has no values");
}

void ValuesController::clampIndex()
{
	if (index > (int)values.size() - 1)
		index = 0;
	else if (index < 0)
		index = values.size() - 1;
}