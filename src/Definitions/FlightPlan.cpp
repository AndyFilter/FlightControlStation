#include <exception>
#include <vector>
using namespace std;

#include "FlightPlan.h"
#include "AcceptanceSystem.h"
#include "Plane.h"
#include "Pilot.h"
#include "Airport.h"
#include "EmergencySystem.h"
#include "RaportCreator.h"

FlightPlan::FlightPlan(Pilot* pilot1, Pilot* pilot2, Plane* plane, Airport* st, Airport* dst, std::vector<Airport*>& emg)
	: startAirport(st), endAirport(dst), plane(plane)
{
	pilots.push_back(pilot1);
	pilots.push_back(pilot2);

	emergency_airport = emg;
}

FlightPlan::FlightPlan(Airport* st, Airport* dst) : startAirport(st), endAirport(dst)
{
}

