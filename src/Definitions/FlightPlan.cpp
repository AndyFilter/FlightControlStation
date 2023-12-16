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

FlightPlan::FlightPlan(Pilot pilot, Plane plane, Airport* st, Airport* dst, vector<Airport*>& emg) {

}

FlightPlan::FlightPlan(Airport* st, Airport* dst) : startAirport(st), endAirport(dst)
{
}

