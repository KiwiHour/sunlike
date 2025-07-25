#include "HomeMenu.h"

void HomeMenu::buildIcons()
{
	index = 1;
	std::vector<Icon> iconNames = {FUNCTIONS, ADJUST, CONFIG};

	for (int i = 0; i < iconNames.size(); i++)
	{
		int x = i * (32 + 2 * PADDING_X) + PADDING_X;
		int y = PADDING_Y;
		icons.push_back(new IconHandler(screen, iconNames[i], x, y));
	}
}

void HomeMenu::draw()
{
	drawHeader();
	for (int i = 0; i < icons.size(); i++)
	{
		icons[i]->draw();
		std::string label = icons[i]->getLabel();
		int x = icons[i]->getPos().first;

		drawIconLabel(label, x, 32 + PADDING_Y + LABEL_Y_OFFSET, i == index);
	}
}

InputResponse HomeMenu::handleInput(SwitchInput input)
{
	if (input == SwitchInput::RIGHT)
		index++;
	if (input == SwitchInput::LEFT)
		index--;
	if (input == SwitchInput::PUSH)
		return InputResponse::GO_INTO;

	clampIndex(icons.size() - 1);
	return InputResponse::STAY;
}

void HomeMenu::onIdle()
{
	index = 1;
}

void HomeMenu::drawIconLabel(const std::string &label, int x0, int y0, bool underline)
{
	int16_t x1, y1;
	uint16_t width, height;
	screen->getTextBounds(label.c_str(), x0, y0, &x1, &y1, &width, &height);

	// Set new x and y pos based on the bounds
	x0 = x1 + ((32 - width) / 2);
	y0 = y1;
	screen->setCursor(x0, y0);
	screen->print(label.c_str());

	if (underline)
	{
		int line_x0 = x0 - 1;
		int line_y0 = y0 + height + 1;
		screen->drawFastHLine(line_x0, line_y0, width, WHITE);
	}
}