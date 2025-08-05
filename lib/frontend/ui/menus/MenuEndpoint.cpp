#include "MenuEndpoint.h"

void MenuEndpoint::draw()
{
	drawHeader();
	screen->println();

	// Check function is assigned
	if (func == nullptr)
	{
		screen->println("\nNo function assigned :/");
		return;
	}

	if (funcExecuted)
	{
		screen->println("\nFunction executed successfully");
		return;
	}

	internalConfigMenu.draw();
}

MenuEndpoint::Args MenuEndpoint::getArgs()
{
	MenuEndpoint::Args args;
	args.reserve(noArgs);

	for (auto controller : internalConfigMenu.controllers)
	{
		for (auto value : controller->values)
		{
			args.push_back(value->getValue());
		}
	}

	return args;
}

bool MenuEndpoint::callFunction()
{
	MenuEndpoint::Args args = getArgs();
	if (noArgs != args.size())
	{
		logCritical("Specified no of arguments doesn't match vector of argument's size");
		return false;
	}

	if (func == nullptr)
	{
		logCritical("Tried to call a non-existent function");
		return false;
	}

	return func(args);
}

InputResponse MenuEndpoint::handleInput(SwitchInput input)
{
	if (func == nullptr || funcExecuted)
	{
		if (input == SwitchInput::PUSH || input == SwitchInput::LEFT)
			return InputResponse::GO_BACK;

		return InputResponse::STAY;
	}

	// Let the hijacking commence
	if (internalConfigMenu.index == internalConfigMenu.controllers.size() - 1)
	{
		// We are on the confirm button
		if (input == SwitchInput::PUSH)
		{
			if (callFunction())
				funcExecuted = true;
			else
				logCritical("Function failed to execute successfully");
		}
	}

	return internalConfigMenu.handleInput(input);
}

void MenuEndpoint::onIdle()
{
	internalConfigMenu.onIdle();
}

void MenuEndpoint::onEnter()
{
	funcExecuted = false;
	internalConfigMenu.onEnter();
}