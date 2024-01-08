#include <exception>
#include <queue>
//using namespace std;

#ifndef __RaportCreator_h__
#define __RaportCreator_h__

 #include "FlightRaport.h"
#include "FlightPlan.h"
 #include "Archive.h"

//class Raport;
class FlightPlan;
//class Archive;
//class RaportCreator;

class RaportCreator
{
	public:
		//std::vector<FlightPlan*> unnamed_FlightPlan;
		
		//Raport* unnamed_Raport;

		void set_Archive_link(Archive* arch);
		void create_raport(FlightPlan& fp); //zmien w diag
		void verify(Raport* a); //zmien w diag
		void send_to_archive(Raport* rap);//zmien w diag
	private:
		//std::queue<Raport*> act; //zmien w diag
		Archive* arch_link;

};

#endif
