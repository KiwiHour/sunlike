#pragma once

#include <Arduino.h>
#include "switch-input.h"

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
	void begin();
	SwitchInput getInput();
};