#include <exception>
//using namespace std;

#ifndef __EmergencySystem_h__
#define __EmergencySystem_h__

#include "FlightPlan.h"

class FlightPlan;
//class EmergencySystem;

class EmergencySystem
{
	public:
		//FlightPlan* unnamed_FlightPlan;

		void notify_emergency_services(FlightPlan fp);
		void set_emergency_airport();
};

#endif
