#ifndef CONFIG_CONTROLLER
#define CONFIG_CONTROLLER

#include "menu.h"

using namespace std;

typedef function<int()> Getter;
typedef function<bool(int)> Setter;

class ConfigController
{
private:
	vector<Getter> getters = {};
	vector<Setter> setters = {};
	size_t padSize;
	char padChar;

public:
	string name;
	vector<int> values = {};
	int index = -1;

	ConfigController(string _name, vector<Getter> _getters, vector<Setter> _setters, size_t _padSize = 0, char _padChar = '?')
	{
		name = _name;
		getters = _getters;
		setters = _setters;
		padSize = _padSize;
		padChar = _padChar;

		values.resize(getters.size());
	}

	string getFormattedValue(int _index)
	{
		string str = to_string(values[_index]);

		// Pad if desired
		if (padSize > 0)
		{
			if (str.length() < padSize)
				str = string(padSize - str.length(), padChar) + str;
		}

		return str;
	}

	// Call the nth getter and return the response
	int get(int _index)
	{
		return getters[_index]();
	}

	// Call the nth setter and update value as well
	int set(int _index, int value)
	{
		bool success = setters[_index](value);

		// TODO: maybe remove this if theres some rubberbanding?
		if (success)
			values[_index] = get(_index);

		return success;
	}

	void refreshValues()
	{
		for (int i = 0; i < values.size(); i++)
		{
			values[i] = get(i);
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
				set(index, values[index]); // TODO: this NOT good coding
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

			values[index] += delta;
		}
	}

	void clampIndex()
	{
		if (index > values.size() - 1)
			index = 0;
		else if (index < 0)
			index = values.size() - 1;
	}
};

#endif