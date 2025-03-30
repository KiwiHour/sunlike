#ifndef HOME_DISPLAY
#define HOME_DISPLAY

#include <vector>
#include "menu.h"
#include "icon_handler.cpp"
#include "icons.h"

#define PADDING_X 4
#define PADDING_Y 15
#define LABEL_Y_OFFSET 6

using namespace std;

class HomeMenu : public Menu
{
public:
	using Menu::Menu;

	void buildIcons()
	{
		vector<Icon> iconNames = {FUNCTIONS, ADJUST, CONFIG};

		for (int i = 0; i < iconNames.size(); i++)
		{
			int x = i * (32 + 2 * PADDING_X) + PADDING_X;
			int y = PADDING_Y;
			icons.push_back(new IconHandler(screen, iconNames[i], x, y));
		}
	}

	virtual void draw()
	{
		string title = "Sunlike";
		int title_x = getCenteredXCoord(title, 0);
		screen.setCursor(title_x, 0);
		screen.print(title.c_str());

		for (int i = 0; i < icons.size(); i++)
		{
			icons[i]->draw();
			string label = icons[i]->getLabel();
			int x = icons[i]->getPos().first;

			drawIconLabel(label, x, 32 + PADDING_Y + LABEL_Y_OFFSET, i == index);
		}
	}

	virtual void handleInput(SwitchInput input)
	{
		if (input == RIGHT)
			index++;
		if (input == LEFT)
			index--;

		if (index < 0)
			index = icons.size() - 1;
		if (index > icons.size() - 1)
			index = 0;
	}
	virtual void handleIdle()
	{
		index = 1;
	}

private:
	vector<IconHandler *> icons = {};

	void drawIconLabel(string label, int x0, int y0, bool underline)
	{
		int16_t x1, y1;
		uint16_t width, height;
		screen.getTextBounds(label.c_str(), x0, y0, &x1, &y1, &width, &height);

		// Set new x and y pos based on the bounds
		x0 = x1 + ((32 - width) / 2);
		y0 = y1;
		screen.setCursor(x0, y0);
		screen.print(label.c_str());

		if (underline)
		{
			int line_x0 = x0 - 1;
			int line_y0 = y0 + height + 1;
			screen.drawFastHLine(line_x0, line_y0, width, WHITE);
		}
	}
};

#endif