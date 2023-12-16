#include <exception>
#include <string>
//using namespace std;

#ifndef __Pilot_h__
#define __Pilot_h__

// #include "FlightPlan.h"

//class FlightPlan;
//class Pilot;

class Pilot
{
	public:
		int pilot_ID;
		//FlightPlan* unnamed_FlightPlan;

		void send_msg(std::string komunikat);
		Pilot(std::string im, std::string naz, int wi);
	private:
		std::string lastName;
		int age;
		static long count;

};

#endif
