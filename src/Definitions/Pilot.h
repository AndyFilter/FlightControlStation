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
	static long count;
	public:
		int pilot_ID;
		//FlightPlan* unnamed_FlightPlan;

		void send_msg(std::string komunikat);
		Pilot(std::string im, std::string naz, int wi);
	public:
		std::string lastName;
		std::string firstName;
		int age;
};

#endif
