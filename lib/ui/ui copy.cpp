#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "icon_handler.cpp"
#include "icons.h"
#include "../switch_input.h"

// Settings
#define OLED_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

using namespace std;

class MenuItem
{
public:
	string label;
	vector<MenuItem *> children = {};
	MenuItem *parent;

	MenuItem(string _label, MenuItem *_parent = nullptr)
	{
		label = _label;
		parent = _parent;
	}

	void addChildren(vector<MenuItem *> _children)
	{
		children.insert(children.end(), _children.begin(), _children.end());
	}
};

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
			// Go back to the root and home menu;
			while (currentMenu->parent != nullptr)
				currentMenu = currentMenu->parent;

			onHomeMenu = true;
			index = 1; // instead do Display.handleIdle()
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
		{

			if ((!onHomeMenu && input == SwitchInput::UP) || (onHomeMenu && input == SwitchInput::RIGHT))
				index++;
			if ((!onHomeMenu && input == SwitchInput::DOWN) || (onHomeMenu && input == SwitchInput::LEFT))
				index--;
			if (input == SwitchInput::PUSH) // Check it has children
			{
				if (onHomeMenu)
				{
					currentMenu = currentMenu->children[index];
					onHomeMenu = false;
				}
				else if (currentMenu->children.size() != 0)
					currentMenu = currentMenu->children[index];

				index = 0;
			}
			if (!onHomeMenu && input == SwitchInput::LEFT) // Check it has a parent
			{
				if (currentMenu->parent->parent == nullptr) // If we were to go back to the root, then go to home menu instead
					onHomeMenu = true;
				else
					currentMenu = currentMenu->parent;

				index = onHomeMenu ? 1 : 0;
			}

			if (index < 0)
				index = onHomeMenu ? homeMenuIcons.size() - 1 : currentMenu->children.size() - 1;
			if (index > (onHomeMenu ? homeMenuIcons.size() - 1 : currentMenu->children.size() - 1))
				index = 0;
		}

		display();

		lastInputMillis = millis();
		screenOn = true;
	}

private:
	Adafruit_SSD1306 screen;

	// State
	const int IDLE_TIMEOUT_MS = 10000;
	MenuItem *currentMenu = nullptr;
	array<IconHandler *, 3> homeMenuIcons;
	int index = 1;
	bool screenOn = true;
	bool onHomeMenu = true;
	unsigned long lastInputMillis;

	void build()
	{
		MenuItem *root = new MenuItem("Root");

		// Root
		MenuItem *config = new MenuItem("Config", root);
		MenuItem *functions = new MenuItem("Functions", root);
		MenuItem *adjust = new MenuItem("Adjust", root);

		// Config
		MenuItem *sunriseConfig = new MenuItem("Sunrise", config);
		MenuItem *sunsetConfig = new MenuItem("Sunset", config);
		MenuItem *duskfallConfig = new MenuItem("Duskfall", config);

		// Functions
		MenuItem *sunriseFunction = new MenuItem("Sunrise", functions);
		MenuItem *duskfallFunction = new MenuItem("Duskfall", functions);

		// Assign children
		root->addChildren({functions, adjust, config});
		config->addChildren({sunriseConfig, sunsetConfig, duskfallConfig});
		functions->addChildren({sunriseFunction, duskfallFunction});

		currentMenu = root;

		// Build home menu
		homeMenuIcons[0] = new IconHandler(screen, FUNCTIONS, 4, 12);
		homeMenuIcons[1] = new IconHandler(screen, ADJUST, 44, 12);
		homeMenuIcons[2] = new IconHandler(screen, CONFIG, 84, 12);
	}

	void display()
	{
		screen.setCursor(0, 0);
		screen.clearDisplay();

		string menuTitle = onHomeMenu ? "Sunlike" : currentMenu->label;
		screen.println(menuTitle.c_str());

		if (onHomeMenu)
		{
			for (int i = 0; i < 3; i++)
			{
				homeMenuIcons[i]->draw();

				// Lord forgive me for this code
				string label = array<string, 3>({"Func", "Adjust", "Conf"})[i];
				int16_t x, y;
				uint16_t w, h;
				screen.getTextBounds(label.c_str(), 4 + (i * (32 + 8)), 52, &x, &y, &w, &h);
				screen.setCursor(x + ((32 - w) / 2), y);
				screen.print(label.c_str());
				if (i == index)
					screen.drawLine(x + ((32 - w) / 2) - 1, y + h + 1, x + ((32 - w) / 2) + w - 1, y + h + 1, WHITE);
			}
		}
		else
		{

			for (int i = 0; i < currentMenu->children.size(); i++)
			{
				if (i == index)
					screen.print("> ");
				else
					screen.print("  ");

				screen.println(currentMenu->children[i]->label.c_str());
			}
		}

		screen.display();
	}
};