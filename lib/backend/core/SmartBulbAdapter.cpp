#include "SmartBulbAdapter.h"

bool SmartBulbAdapter::errorCodeToBool(int errorCode)
{
	return errorCode == 0;
}

void SmartBulbAdapter::ensureSnapshotFreshness()
{
	if (!isSnapshotFresh())
		createSnapshot();
}

void SmartBulbAdapter::createSnapshot()
{
	attributeSnapshot = core->getAttributes();
	attributeSnapshotMillis = millis();
}

bool SmartBulbAdapter::isSnapshotFresh()
{
	return (millis() - attributeSnapshotMillis < freshSnapshotThreshold_ms);
}

// Static
bool SmartBulbAdapter::isDeviceOn()
{
	HTTPClient http;
	http.begin(("http://" + bulbIp).c_str());
	int responseCode = http.GET();
	http.end();

	return responseCode == 200;
}

void SmartBulbAdapter::begin()
{
	prevMillis = millis();
	core = new L530(bulbIp);
}

void SmartBulbAdapter::tick()
{
	core->incrementCookieAge(millis() - prevMillis);
	prevMillis = millis();
}

int SmartBulbAdapter::getPowerState()
{
	ensureSnapshotFreshness();
	return (int)(attributeSnapshot.powerState);
}
bool SmartBulbAdapter::setPowerState(int state)
{
	if (state == 1)
		core->turnOn();
	else if (state == 0)
		core->turnOff();
	else
		return false;

	return true;
}

int SmartBulbAdapter::getBrightness()
{
	ensureSnapshotFreshness();
	return attributeSnapshot.brightness;
}
bool SmartBulbAdapter::setBrightness(int brightness)
{
	return errorCodeToBool(core->setBrightness(brightness));
}

int SmartBulbAdapter::getColorTemperature()
{
	ensureSnapshotFreshness();
	return attributeSnapshot.colorTemperature;
}
bool SmartBulbAdapter::setColorTemperature(int temperature)
{
	return errorCodeToBool(core->setColorTemperature(temperature));
}

int SmartBulbAdapter::getHue()
{
	ensureSnapshotFreshness();
	return attributeSnapshot.hue;
}
bool SmartBulbAdapter::setHue(int hue)
{
	return errorCodeToBool(core->setHue(hue));
}

int SmartBulbAdapter::getSaturation()
{
	ensureSnapshotFreshness();
	return attributeSnapshot.saturation;
}
bool SmartBulbAdapter::setSaturation(int saturation)
{
	return errorCodeToBool(core->setSaturation(saturation));
}