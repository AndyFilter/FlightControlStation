#pragma once

#include <exception>
#include <vector>
//using namespace std;

//#ifndef __FlightPlan_h__
//#define __FlightPlan_h__

// #include "AcceptanceSystem.h"
// #include "Plane.h"
//#include "Pilot.h"
#include "Airport.h"
// #include "EmergencySystem.h"
// #include "RaportCreator.h"

class AcceptanceSystem;
class Plane;
class Pilot;
class Airport;
class EmergencySystem;
class RaportCreator;

class FlightPlan
{
	public:
		Airport* startAirport; // start;
		Airport* endAirport; // destination;
		std::vector<Airport*> emergency_airport;
		Plane* plane;
		std::vector<Pilot*> pilots;
		//std::vector<Airport*> unnamed_Airport;
		//EmergencySystem* unnamed_EmergencySystem;
		//RaportCreator* unnamed_RaportCreator;

	public:

		FlightPlan(Pilot* pilot1, Pilot* pilot2, Plane* plane, Airport* st, Airport* dst, std::vector<Airport*>& emg);
		FlightPlan(Airport* st, Airport* dst);
};


// Old Class

//class FlightPlan {
//public:
//	Airport* startAirport = nullptr;
//	Airport* endAirport = nullptr;
//
//public:
//
//	FlightPlan(Airport* startAirport = nullptr, Airport* endAirport = nullptr)
//		: startAirport(startAirport), endAirport(endAirport)
//	{
//	}
//};


//#endif
