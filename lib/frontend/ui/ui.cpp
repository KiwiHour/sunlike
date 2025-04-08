#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "menu.cpp"
#include "menus/menu_endpoint.cpp"
#include "menus/home.cpp"
#include "menus/submenu.cpp"
#include "menus/config_menu.cpp"
#include "../switch_input.h"

// Settings
#define OLED_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

using namespace std;

// TODO: remove

int testVal = 69;
bool testSetter(int x)
{
	testVal = x;
	return true;
}
int testGetter() { return testVal; }

class UI
{

public:
	void begin()
	{
		screen = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
		screen->begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

		screen->setTextSize(1);
		screen->setCursor(0, 0);
		screen->setTextColor(WHITE);
		screen->clearDisplay();

		build();
		display();
		lastInputMillis = millis();
	}

	/* Run this as much as possible (within reason, maybe every 10-50ms)*/
	void tick()
	{

		if (millis() - lastInputMillis > IDLE_TIMEOUT_MS)
		{
			// Go back to the home menu;
			currentMenu->onIdle(); // Make sure to handle the idle of the menu before going back to home
			while (currentMenu->parent != nullptr)
				currentMenu = currentMenu->parent;

			// Go idle (turn off screen)
			currentMenu->onIdle();
			screen->clearDisplay();
			screen->display();
			screenOn = false;
		};
	}

	void handleInput(SwitchInput input)
	{

		if (input == SwitchInput::NONE)
			return;

		if (screenOn)
		{
			InputResponse response = currentMenu->handleInput(input);
			if (response == InputResponse::GO_INTO)
			{
				currentMenu = currentMenu->children[currentMenu->index];
				currentMenu->index = 0;
				currentMenu->onEnter();
			}
			else if (response == InputResponse::GO_BACK)
			{
				currentMenu = currentMenu->parent;
			}
		}

		display();
		screenOn = true;

		lastInputMillis = millis();
	}

private:
	Adafruit_SSD1306 *screen;

	// State
	Menu *currentMenu = nullptr;
	const int IDLE_TIMEOUT_MS = 10000;
	bool screenOn = true;
	unsigned long lastInputMillis;

	/*
		##########################################
		##		BUILDING THE MENU STRUCTURE		##
		##########################################
	*/

	void build()
	{
		Menu::setScreen(screen);

		HomeMenu *home = new HomeMenu("Sunlike");
		home->buildIcons();

		// Home
		Menu *functions = buildFunctionsMenu(home);
		Menu *adjust = buildAdjustMenu(home);
		Menu *config = buildConfigMenu(home);
		home->addChildren({functions, adjust, config});

		currentMenu = home;
	}

	Menu *buildFunctionsMenu(Menu *home)
	{
		SubMenu *functions = new SubMenu("Functions", home);

		MenuEndpoint *startDuskfall = new MenuEndpoint("Start duskfall", functions);
		MenuEndpoint *pauseSunset = new MenuEndpoint("Pause sunset", functions);
		functions->addChildren({startDuskfall, pauseSunset});

		return functions;
	}

	Menu *buildAdjustMenu(Menu *home)
	{
		ConfigMenu *adjust = new ConfigMenu("Adjustments", home);

		adjust->addControllers({
			new ConfigController("Brightness", {new ValueController("brightness", 1, 100)}),

		});

		return adjust;
	}

	Menu *buildConfigMenu(Menu *home)
	{
		SubMenu *config = new SubMenu("Config", home);

		// Config submenus
		SubMenu *sunriseConfig = new SubMenu("Sunrise", config);
		SubMenu *sunsetConfig = new SubMenu("Sunset", config);
		SubMenu *duskfallConfig = new SubMenu("Duskfall", config);
		config->addChildren({sunriseConfig, sunsetConfig, duskfallConfig});

		// TODO: add ConfigMenus for each of these submenus (need duration and start time, maybe stuff like ending brightness and hue? prob not)

		return config;
	}

	/*
		###########################################
		## /END BUILDING THE MENU STRUCTURE	END/ ##
		###########################################
	*/

	void display()
	{
		screen->clearDisplay();
		currentMenu->draw();
		screen->display();
	}
};