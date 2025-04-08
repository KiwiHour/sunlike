#include <vector>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>

#include "../frontend/input_handler.cpp"
#include "../frontend/ui/ui.cpp"
#include "../backend/sunlike_controller.cpp"
#include "../include/network_credentials.h"
#include "../backend/state_controller.cpp"

using namespace std;

UI *ui;
InputHandler inputHandler;
SmartBulbAdapter *bulb;
SunlikeController *sunlike;

bool connectOverWiFi()
{
	WiFi.begin(NETWORK_SSID, NETWORK_KEY);
	Serial.printf("Connecting to %s", NETWORK_SSID);

	int tries = 0;
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		if (tries > 50)
		{
			Serial.println("\nFailed to connect");
			return false;
		}
		delay(250);
	}

	Serial.println();

	return true;
}

void setup()
{
	Serial.begin(115200);
	delay(2000);

	connectOverWiFi();
	ui = new UI();
	bulb = new SmartBulbAdapter();
	state = new StateController();

	state->addValue("brightness", bind(&SmartBulbAdapter::getBrightness, bulb), bind(&SmartBulbAdapter::setBrightness, bulb, placeholders::_1));

	bulb->begin();
	sunlike = new SunlikeController(bulb);

	ui->begin();
	inputHandler.begin();
}

void loop()
{
	SwitchInput input = inputHandler.getInput();

	ui->handleInput(input);
	ui->tick();

	// sunlike.tick(input == SwitchInput::PUSH);

	delay(10);
}