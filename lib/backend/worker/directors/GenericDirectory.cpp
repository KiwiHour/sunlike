#include "GenericDirector.h"

std::pair<int, int> GenericDirector::getHoursMinutes()
{
	// I think this should instead be ran once during setup, and then let the hardware clock handle it from there onwards

	HTTPClient http;
	http.begin("http://worldtimeapi.org/api/ip");
	int responseCode = http.GET();

	if (responseCode != 200)
	{
		Serial.println("Failed to get time using NTP");
		return {};
	}

	String payload = http.getString();

	nlohmann::json response = nlohmann::json::parse(payload);

	if (!response.contains("datetime"))
	{
		Serial.println("Missing data from NTP response");
		return {};
	}

	// Parse datetime to extract hours and minutes
	std::string datetime = response["datetime"];
	int hours = std::stoi(datetime.substr(datetime.find('T') + 1, 2));
	int minutes = std::stoi(datetime.substr(datetime.find('T') + 4, 2));

	return {hours, minutes};
}