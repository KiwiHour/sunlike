#include "IconHandler.h"

void IconHandler::draw()
{
	screen->drawBitmap(x, y, bitmap.data(), 32, 32, WHITE);
}

std::pair<int, int> IconHandler::getPos()
{
	return {x, y};
}

std::string IconHandler::getLabel()
{
	switch (icon)
	{
	case FUNCTIONS:
		return "Funcs";
	case CONFIG:
		return "Config";
	case ADJUST:
		return "Adjust";
	default:
		return "Unknown";
	}
}