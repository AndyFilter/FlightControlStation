#include <exception>
using namespace std;

#include "FlightRaport.h"
#include "FlightPlan.h"
#include "Raport.h"
#include "Pilot.h"
#include "Plane.h"


FlightRaport::FlightRaport(FlightPlan fp, const Airport *const fd): 
Raport::Raport(fp.plane->specifications), start(fp.startAirport), planed_destination(fp.endAirport), emergency_airport(fp.emergency_airport), final_destination(fd)
{
	pilot_ID[0] = fp.pilots[0]->pilot_ID;
	pilot_ID[1] = fp.pilots[1]->pilot_ID;

}

void FlightRaport::print(ofstream& output) {
	output << "LOT" << endl << "Samolot: " << this->plane_specs << endl;
	output << "Piloci: " << pilot_ID[0] << ", " << pilot_ID[1] << endl;
	output << "Start:" << start << endl;
	output << "Destynacja:" << final_destination->identifier;
	if (final_destination != planed_destination) output << " (zmienione)";
	output << endl;

	int limit = emergency_airport.size();
	for (int i = 0; i < limit; i++)
		output << emergency_airport[i]->identifier<<"\t";
	output << endl;
}

