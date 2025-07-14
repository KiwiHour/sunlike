#include "MenuEndpoint.h"

void MenuEndpoint::setFunction(std::function<bool()> _func)
{
	func = _func;
}

void MenuEndpoint::draw()
{
	drawHeader();
	screen->println();

	// Check function is assigned
	if (func == nullptr)
	{
		screen->println("No function assigned :/");
		return;
	}

	bool success = func();
	std::string message = success ? "Success :)" : "Failed :(";

	screen->println(message.c_str());
}

InputResponse MenuEndpoint::handleInput(SwitchInput input)
{
	if (input == SwitchInput::LEFT || input == SwitchInput::PUSH)
		return InputResponse::GO_BACK;

	return InputResponse::STAY;
}