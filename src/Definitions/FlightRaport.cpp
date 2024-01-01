#include <exception>
using namespace std;

#include "FlightRaport.h"
#include "FlightPlan.h"
#include "Raport.h"
#include "Pilot.h"



FlightRaport::FlightRaport(FlightPlan fp, const Airport *const fd): 
start(fp.startAirport), planed_destination(fp.endAirport), emergency_airport(fp.emergency_airport), final_destination(fd)
{
	 
	pilot_ID[0] = fp.pilots[0]->pilot_ID;
	pilot_ID[1] = fp.pilots[1]->pilot_ID;

}

//void FlightRaport::print() {
//
//}

