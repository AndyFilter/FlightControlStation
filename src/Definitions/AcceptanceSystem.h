#include <exception>
#include <queue>
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

// SINGLETON
class AcceptanceSystem
{
	public:
		std::queue<FlightPlan*> flightplans;
		int queued = 0;

		void submitFlightPlan(Pilot* pilot, Plane* plane, Airport* st, Airport* dst);
		void submitFlightPlan(Pilot* pilot, Plane* plane, Airport* st, Airport* dst, std::vector<Airport*> emg);
		void addFlightPlan(FlightPlan fp);

		// Get singleton
		static AcceptanceSystem* Instance() {
			static AcceptanceSystem* singleton;
			if (!singleton)
				singleton = new AcceptanceSystem();

			return singleton;
		}

		void verify();

	private:
		std::queue<FlightPlan*> FPVector;
};

#endif
