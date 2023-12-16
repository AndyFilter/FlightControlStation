#include <exception>
//using namespace std;

#ifndef __FlightRaport_h__
#define __FlightRaport_h__

#include "FlightPlan.h"
#include "Raport.h"
#include "Airport.h"

//class FlightPlan;
// class Raport;
class FlightRaport;

class FlightRaport: public Raport
{
	public:

		FlightRaport(FlightPlan fp, const Airport *const fd);
	private:
		const Airport * start;
		const Airport * planed_destination;
		const Airport * emergency_airport;
		const Airport * final_destination;
		int pilot_ID[2];

};

#endif
