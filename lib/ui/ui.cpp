#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "menu.cpp"
#include "menus/home.cpp"
#include "menus/submenu.cpp"
#include "../switch_input.h"

// Settings
#define OLED_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

using namespace std;

class UI
{

public:
	void begin()
	{
		screen = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
		screen.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

		screen.setTextSize(1);
		screen.setCursor(0, 0);
		screen.setTextColor(WHITE);
		screen.clearDisplay();

		build();
		display();
		lastInputMillis = millis();
	}

	/* Run this as much as possible (within reason, maybe every 10-50ms)*/
	void tick()
	{

		if (millis() - lastInputMillis > IDLE_TIMEOUT_MS)
		{
			// TODO: Go back to the root and home menu;
			while (currentMenu->parent != nullptr)
				currentMenu = currentMenu->parent;

			currentMenu->handleIdle();
			screen.clearDisplay();
			screen.display();
			screenOn = false;
		};
	}

	void handleInput(SwitchInput input)
	{

		if (input == NONE)
			return;

		if (screenOn)
			currentMenu->handleInput(input);

		display();

		lastInputMillis = millis();
		screenOn = true;
	}

private:
	Adafruit_SSD1306 screen;

	// State
	Menu *currentMenu = nullptr;
	const int IDLE_TIMEOUT_MS = 10000;
	bool screenOn = true;
	unsigned long lastInputMillis;

	void build()
	{
		HomeMenu *home = new HomeMenu(screen, "Sunlike");
		home->buildIcons();

		Menu *functions = new SubMenu(screen, "Functions", home);
		Menu *adjust = new SubMenu(screen, "Adjustments", home);
		Menu *config = new SubMenu(screen, "Config", home);

		home->addChildren({functions, adjust, config});

		currentMenu = home;
	}

	void display()
	{
		screen.clearDisplay();
		currentMenu->draw();
		screen.display();
	}
};