#ifndef SWITCH_INPUT_H
#define SWITCH_INPUT_H

// TODO:
// If i wanted to add the ability to hold down, I could make new enum values HOLDING_UP, HOLDING_DOWN, HOLDING_LEFT, HOLDING_RIGHT, HOLDING_PUSH
// Unsure how much NONE is required for functionality later down the road, but this way holding can delegated to later controllers
// eg. a ConfigController may want to know whats being held down, that way you can increase/decrease a value by holding rather than spamming inputs

enum class SwitchInput
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	PUSH,

	HOLDING_PUSH,
	HOLDING_UP,
	HOLDING_DOWN,
	HOLDING_RIGHT,
	HOLDING_LEFT
};

#endif