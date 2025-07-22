#include "ConfigMenu.h"

void ConfigMenu::draw()
{
	drawHeader();
	screen->print("\n");

	// TODO: Draw the value(s) on the far right of the row, will need text bounds prob. delegate this to the controller me thinks

	for (int i = 0; i < controllers.size(); i++)
	{
		ConfigController *controller = controllers[i];
		if (i == index)
		{
			// If controller selected (aka, value being "looked at"), then make ">" bold
			if (controller->index != -1)
			{
				int x = screen->getCursorX();
				screen->setCursor(x + 1, screen->getCursorY());
				screen->print("> ");
				screen->setCursor(x - 1, screen->getCursorY());
				screen->print("> ");
				screen->setCursor(x, screen->getCursorY());
			}
			screen->print("> ");
		}
		else
			screen->print("  ");

		screen->print(controller->name.c_str());

		// Justify values to the righthand side
		// PADDING = MAX_CHARS - SELECTOR CHAR (>) PADDING - CONTROLLER NAME SIZE - PADDING BETWEEN VALUES - SIZE OF VALUES
		int paddingChars = 21 - 2 - controller->name.length() - (controller->values.size() - 1);
		std::vector<std::string> formattedValues = {};

		for (int j = 0; j < controller->values.size(); j++)
			formattedValues.push_back(controller->values[j]->getFormattedValue());

		for (int j = 0; j < controller->values.size(); j++)
			paddingChars -= formattedValues[j].length();

		for (int j = 0; j < paddingChars; j++)
			screen->print(" ");

		for (int j = 0; j < controller->values.size(); j++)
		{
			// Highlight the selected controller's current value thats being "looked at"
			if (i == index && j == controller->index)
				screen->setTextColor(BLACK, WHITE);

			screen->print(formattedValues[j].c_str());
			screen->setTextColor(WHITE);

			// Padding only for between values, not at the end
			if (j < controller->values.size() - 1)
				screen->print(" ");
		}

		screen->println();
	}
}

void ConfigMenu::addControllers(const std::vector<ConfigController *> &_controllers)
{
	controllers.insert(controllers.end(), _controllers.begin(), _controllers.end());
}

InputResponse ConfigMenu::handleInput(SwitchInput input)
{
	// Change how input is handled, depending on if a controller is selected or not
	if (controllers[index]->index != -1)
		controllers[index]->handleInput(input);
	else
	{
		if (input == SwitchInput::UP)
			index--;
		if (input == SwitchInput::DOWN)
			index++;
		if (input == SwitchInput::LEFT)
			return InputResponse::GO_BACK;
		if (input == SwitchInput::PUSH)
			controllers[index]->index = 0;

		clampIndex(controllers.size() - 1);
	}

	return InputResponse::STAY;
}

void ConfigMenu::onIdle()
{
	index = 0;
	// Unselect value on each controller
	for (ConfigController *controller : controllers)
		controller->index = -1;
}

void ConfigMenu::onEnter()
{
	// Refresh the state
	for (const auto controller : controllers)
	{
		for (const auto value : controller->values)
		{
			state.fetch(value->stateName);
			value->updateInternalValue();
		}
	}
}