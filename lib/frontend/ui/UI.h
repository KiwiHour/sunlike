#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "ui/menus/menus.h"
#include "switch-input.h"

// Settings
#define OLED_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

class UI
{

public:
	void begin();

	/* Run this as much as possible (within reason, maybe every 10-50ms)*/
	void tick();
	void handleInput(SwitchInput input);

private:
	Adafruit_SSD1306 *screen;

	// State
	Menu *currentMenu = nullptr;
	const int IDLE_TIMEOUT_MS = 10000;
	bool screenOn = true;
	unsigned long lastInputMillis;

	void build();
	Menu *buildFunctionsMenu(Menu *home);
	Menu *buildAdjustMenu(Menu *home);
	Menu *buildConfigMenu(Menu *home);

	void display();
};