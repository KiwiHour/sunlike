#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "../lib/input_handler.cpp"
#include "ui.cpp"

using namespace std;

UI ui;
InputHandler inputHandler;

void setup()
{
	Serial.begin(115200);
	delay(2000);
	ui.begin();
	inputHandler.begin();
}

void loop()
{
	SwitchInput input = inputHandler.getInput();

	ui.handleInput(input);
	ui.tick();

	delay(10);
}