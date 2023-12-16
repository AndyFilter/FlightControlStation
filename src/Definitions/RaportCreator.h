#include <exception>
#include <vector>
//using namespace std;

#ifndef __RaportCreator_h__
#define __RaportCreator_h__

// #include "Raport.h"
#include "FlightPlan.h"
// #include "Archive.h"

//class Raport;
class FlightPlan;
//class Archive;
//class RaportCreator;

class RaportCreator
{
	public:
		std::vector<FlightPlan*> unnamed_FlightPlan;
		//Archive* unnamed_Archive;
		//Raport* unnamed_Raport;

		void create_raport(FlightPlan fp);
		void send_to_archive();
	private:
		//Raport* act;

};

#endif
