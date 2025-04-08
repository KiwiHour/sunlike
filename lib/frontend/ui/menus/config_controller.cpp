#ifndef CONFIG_CONTROLLER
#define CONFIG_CONTROLLER

#include "../menu.h"
#include "../backend/state_controller.h"
#include "value_controller.cpp"

using namespace std;

class ConfigController
{
private:
	int holdCount = 0; // Amount of instances of being held down

public:
	string name;
	int index = -1;
	vector<ValueController *> values = {};

	ConfigController(string _name, vector<ValueController *> _values)
	{
		name = _name;
		values = _values;
	}

	void handleInput(SwitchInput input)
	{
		if (input == SwitchInput::PUSH)
		{
			// If none selected, select first one. Else, unselect
			if (index == -1)
				index = 0;
			else
			{
				state->flush(); // Flush confirmed dirty changes
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
		values[index]->handleInput(input);
	}

	void clampIndex()
	{
		if (index > (int)values.size() - 1)
			index = 0;
		else if (index < 0)
			index = values.size() - 1;
	}
};

#endif