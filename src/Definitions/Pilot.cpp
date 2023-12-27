#include <exception>
#include <string>
using namespace std;

#include "../LogicHelper.h"
#include "Pilot.h"
//#include "FlightPlan.h"

// More like a "receive message"
void Pilot::send_msg(string komunikat) {
	messages.push_back({ firstName + " " + lastName, komunikat});
	//throw "Not yet implemented";
}

Pilot::Pilot(string im, string naz, int wi) : lastName(naz), firstName(im), age(wi), pilot_ID(count++) {
}

