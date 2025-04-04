#ifndef CONFIG_CONTROLLER
#define CONFIG_CONTROLLER

#include "menu.h"
#include "value_controller.cpp"

using namespace std;

class ConfigController
{
private:
public:
	string name;
	int index = -1;
	vector<ValueController *> values = {};

	ConfigController(string _name, vector<ValueController *> _values)
	{
		name = _name;
		values = _values;
	}

	void refreshValues()
	{
		for (ValueController *value : values)
		{
			value->fetch();
		}
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
				values[index]->flush();
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
		if (input == SwitchInput::UP || input == SwitchInput::DOWN || input == SwitchInput::HOLDING_UP || input == SwitchInput::HOLDING_DOWN)
		{
			int delta = (input == SwitchInput::UP || input == SwitchInput::HOLDING_UP) ? 1 : -1;

			values[index]->adjust(delta);
			values[index]->circularClamp();
		}
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