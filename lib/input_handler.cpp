#include <Arduino.h>
#include "switch_input.h"

class InputHandler
{
private:
	const int PUSH_PIN = 5;
	const int UP_PIN = 11;
	const int RIGHT_PIN = 10;
	const int DOWN_PIN = 9;
	const int LEFT_PIN = 6;

	SwitchInput previousInput = NONE;

public:
	void begin()
	{
		pinMode(PUSH_PIN, INPUT_PULLUP);
		pinMode(UP_PIN, INPUT_PULLUP);
		pinMode(RIGHT_PIN, INPUT_PULLUP);
		pinMode(DOWN_PIN, INPUT_PULLUP);
		pinMode(LEFT_PIN, INPUT_PULLUP);
	}

	SwitchInput getInput()
	{
		SwitchInput input;

		if (digitalRead(PUSH_PIN) == 0)
			input = PUSH;
		else if (digitalRead(UP_PIN) == 0)
			input = UP;
		else if (digitalRead(RIGHT_PIN) == 0)
			input = RIGHT;
		else if (digitalRead(DOWN_PIN) == 0)
			input = DOWN;
		else if (digitalRead(LEFT_PIN) == 0)
			input = LEFT;
		else
			input = NONE;

		if (input != NONE && previousInput == NONE)
		{
			previousInput = input;
			return input;
		}

		if (input == NONE && previousInput != NONE)
		{
			previousInput = NONE;
		}

		return NONE;
	}
};