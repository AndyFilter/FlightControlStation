#include <exception>
#include <vector>
//using namespace std;

#ifndef __AcceptanceSystem_h__
#define __AcceptanceSystem_h__

//#include "FlightPlan.h"
//#include "Pilot.h"
//#include "Plane.h"
// #include "Airport.h"

class FlightPlan;
class Pilot;
class Plane;
class Airport;
class AcceptanceSystem;

class AcceptanceSystem
{
	public:
		std::vector<FlightPlan*> flightplans;

		void submitFlightPlan(Pilot pilot, Plane plane, Airport* st, Airport* dst, Airport* emg[] );
		void addFlightPlan(FlightPlan fp);
	private:
		FlightPlan* FPVector[];

		void verify();
};

#endif
