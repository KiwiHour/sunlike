#pragma once

#include <vector>
#include "ui/Menu.h"
#include "ui/IconHandler.h"
#include "icons.h"

#define PADDING_X 4
#define PADDING_Y 15
#define LABEL_Y_OFFSET 6

class HomeMenu : public Menu
{
public:
	using Menu::Menu;

	void buildIcons();
	virtual void draw();
	virtual InputResponse handleInput(SwitchInput input);
	virtual void onIdle();

private:
	std::vector<IconHandler *> icons = {};

	void drawIconLabel(const std::string &label, int x0, int y0, bool underline);
};