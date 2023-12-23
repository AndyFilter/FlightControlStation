#include <exception>
#include <vector>
using namespace std;

#include "AcceptanceSystem.h"
#include "FlightPlan.h"
#include "Pilot.h"
#include "Plane.h"

void AcceptanceSystem::submitFlightPlan(Pilot* pilot, Plane* plane, Airport* st, Airport* dst, std::vector<Airport*> emg) {
	queued++;
	throw "Not yet implemented";
}

void AcceptanceSystem::submitFlightPlan(Pilot* pilot, Plane* plane, Airport* st, Airport* dst) {
	queued++;
	//throw "Not yet implemented";
}

void AcceptanceSystem::addFlightPlan(FlightPlan fp) {
	queued--;
	throw "Not yet implemented";
}

void AcceptanceSystem::verify() {
	throw "Not yet implemented";
}

