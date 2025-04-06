#ifndef ENDPOINT
#define ENDPOINT

#include "../menu.h"

class MenuEndpoint : public Menu
{
public:
	function<bool()> func = nullptr;

	using Menu::Menu;

	void setFunction(function<bool()> _func)
	{
		func = _func;
	}

	// Will run the assigned function upon being drawn
	virtual void draw()
	{
		drawTitle();
		screen->println();

		// Check function is assigned
		if (func == nullptr)
		{
			screen->println("No function assigned :/");
			return;
		}

		bool success = func();
		string message = success ? "Success :)" : "Failed :(";

		screen->println(message.c_str());
	}

	virtual InputResponse handleInput(SwitchInput input)
	{
		if (input == SwitchInput::LEFT || input == SwitchInput::PUSH)
			return InputResponse::GO_BACK;

		return InputResponse::STAY;
	}

	virtual void onIdle() {}
};

#endif