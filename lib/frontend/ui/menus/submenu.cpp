#include "SubMenu.h"

void SubMenu::draw()
{
	drawHeader();
	screen->print("\n");

	for (int i = 0; i < children.size(); i++)
	{
		if (i == index)
			screen->print("> ");
		else
			screen->print("  ");

		screen->println(children[i]->title.c_str());
	}
}

InputResponse SubMenu::handleInput(SwitchInput input)
{
	if (input == SwitchInput::UP)
		index--;
	if (input == SwitchInput::DOWN)
		index++;
	if (input == SwitchInput::PUSH)
		return InputResponse::GO_INTO;
	if (input == SwitchInput::LEFT)
		return InputResponse::GO_BACK;

	clampIndex(children.size() - 1);
	return InputResponse::STAY;
}

void SubMenu::onIdle()
{
	index = 0;
}