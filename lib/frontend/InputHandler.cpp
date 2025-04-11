#include <Arduino.h>
#include "../../include/switch-input.h"

class InputHandler
{
private:
	const int PUSH_PIN = 5;
	const int UP_PIN = 9;
	const int RIGHT_PIN = 10;
	const int DOWN_PIN = 11;
	const int LEFT_PIN = 6;

	SwitchInput previousInput = SwitchInput::NONE;

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
			input = SwitchInput::PUSH;
		else if (digitalRead(UP_PIN) == 0)
			input = SwitchInput::UP;
		else if (digitalRead(RIGHT_PIN) == 0)
			input = SwitchInput::RIGHT;
		else if (digitalRead(DOWN_PIN) == 0)
			input = SwitchInput::DOWN;
		else if (digitalRead(LEFT_PIN) == 0)
			input = SwitchInput::LEFT;
		else
			input = SwitchInput::NONE;

		if (input != SwitchInput::NONE && previousInput == SwitchInput::NONE)
		{
			previousInput = input;
			return input;
		}

		if (input == SwitchInput::NONE && previousInput != SwitchInput::NONE)
		{
			previousInput = SwitchInput::NONE;
		}

		if (input == SwitchInput::PUSH)
			return SwitchInput::HOLDING_PUSH;
		if (input == SwitchInput::LEFT)
			return SwitchInput::HOLDING_LEFT;
		if (input == SwitchInput::RIGHT)
			return SwitchInput::HOLDING_RIGHT;
		if (input == SwitchInput::UP)
			return SwitchInput::HOLDING_UP;
		if (input == SwitchInput::DOWN)
			return SwitchInput::HOLDING_DOWN;

		return SwitchInput::NONE;
	}
};