#pragma once

#include <vector>
#include <memory>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "utils.h"

#include "switch-input.h"

enum class InputResponse
{
	GO_INTO,
	GO_BACK,
	STAY
};

class Menu
{
public:
	static Adafruit_SSD1306 *screen;
	std::string title;
	std::vector<Menu *> children = {};
	Menu *parent;
	int index = 0;

	Menu(const std::string &_title, Menu *_parent = nullptr)
		: title(_title), parent(_parent) {}
	static void setScreen(Adafruit_SSD1306 *_screen);

	void addChildren(const std::vector<Menu *> &_children);
	void clampIndex(int upper);

	// Centered along the x-axis of the entire screen
	int getCenteredXCoord(const std::string &text);
	int getRightJustifiedXCoord(const std::string &text);

	void drawHeader();
	virtual void draw() = 0;
	virtual InputResponse handleInput(SwitchInput input) = 0;
	virtual void onIdle() = 0;
	virtual void onEnter() {}
};