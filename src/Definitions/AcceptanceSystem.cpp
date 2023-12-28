#include <exception>
#include <vector>
using namespace std;

#include "AcceptanceSystem.h"
#include "FlightPlan.h"
#include "Pilot.h"
#include "Plane.h"
#include "../LogicHelper.h"

//void AcceptanceSystem::submitFlightPlan(Pilot* pilot1, Pilot* pilot2, Plane* plane, Airport* st, Airport* dst) {
//	queued_count++;
//	auto fp = new FlightPlan(pilot1, pilot2, plane, st, dst);
//	FPVector.push(fp)
//	//throw "Not yet implemented";
//}

void AcceptanceSystem::submitFlightPlan(Pilot* pilot1, Pilot* pilot2, Plane* plane, Airport* st, Airport* dst, std::vector<Airport*> emg) {
	queued_count++;
	auto fp = new FlightPlan(pilot1, pilot2, plane, st, dst, emg);
	q_flightplans.push(fp);
	//throw "Not yet implemented";
}

//void AcceptanceSystem::addFlightPlan(FlightPlan fp) {
//	queued_count--;
//	//throw "Not yet implemented";
//}

FlightPlan* AcceptanceSystem::verify(bool succ) {
	queued_count--;
	auto top = q_flightplans.front();
	q_flightplans.pop();
	if (!succ) {
		delete top;
		return nullptr;
	}
	top->plane->flightPlan = top;
	planes.push_back(top->plane);
	flightplans.push_back(top);
	return top;
	//throw "Not yet implemented";
}

