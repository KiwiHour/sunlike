#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "../frontend/ui/menus/menus.h"
#include "../../../include/switch-input.h"

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

		adjust->addControllers({new ConfigController("Power state", {new BoolMenuValue("bulb_power_state", "on", "off")})});
		adjust->addControllers({new ConfigController("Brightness", {new GenericMenuValue("bulb_brightness", 1, 100, "%")})});
		adjust->addControllers({new ConfigController("Color Temp", {new GenericMenuValue("bulb_color_temperature", 2500, 6500, "k")})});
		adjust->addControllers({new ConfigController("Hue", {new GenericMenuValue("bulb_hue", 0, 360)})});
		adjust->addControllers({new ConfigController("Saturation", {new GenericMenuValue("bulb_saturation", 1, 100, "%")})});

		return adjust;
	}

	Menu *buildConfigMenu(Menu *home)
	{
		SubMenu *config = new SubMenu("Config", home);

		// Config submenus
		ConfigMenu *sunriseConfig = new ConfigMenu("Sunrise", config);
		ConfigMenu *sunsetConfig = new ConfigMenu("Sunset", config);
		ConfigMenu *duskfallConfig = new ConfigMenu("Duskfall", config);
		config->addChildren({sunriseConfig, sunsetConfig, duskfallConfig});

		// TODO: add ConfigMenus for each of these submenus (need duration and start time, maybe stuff like ending brightness and hue? prob not)
		sunriseConfig->addControllers({
			new ConfigController("Start time", {new TimeMenuValue("sunrise_start_hour", TimeUnit::HOUR),
												new TimeMenuValue("sunrise_start_minute", TimeUnit::MINUTE)}),
			new ConfigController("Duration", {new DurationMenuValue("sunrise_duration_hour", TimeUnit::HOUR, 0, 6),
											  new DurationMenuValue("sunrise_duration_minute", TimeUnit::MINUTE, 0, 59)}),
		});
		sunsetConfig->addControllers({
			new ConfigController("Start time", {new TimeMenuValue("sunset_start_hour", TimeUnit::HOUR),
												new TimeMenuValue("sunset_start_minute", TimeUnit::MINUTE)}),
			new ConfigController("Duration", {new DurationMenuValue("sunset_duration_hour", TimeUnit::HOUR, 0, 6),
											  new DurationMenuValue("sunset_duration_minute", TimeUnit::MINUTE, 0, 59)}),
		});
		duskfallConfig->addControllers({
			new ConfigController("Duration", {new DurationMenuValue("duskfall_duration_hour", TimeUnit::HOUR, 0, 2),
											  new DurationMenuValue("duskfall_duration_minute", TimeUnit::MINUTE, 0, 59)}),
		});

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