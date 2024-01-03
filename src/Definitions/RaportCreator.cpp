#include <exception>
#include <vector>
using namespace std;

#include "RaportCreator.h"
#include "Raport.h"
#include "FlightPlan.h"
#include "Archive.h"
//#include "FlightRaport.h"

void RaportCreator::set_Archive_link(Archive* arch) {
	arch_link = arch;
}

void RaportCreator::create_raport(FlightPlan& fp) {
	Raport* rap =new FlightRaport(fp, nullptr);

	verify(rap);
}

void RaportCreator::verify(Raport* rap) { //Symuluje zatwierdzenie planu przez pilota
 //Zatwierdzanie
	if (true)send_to_archive(rap);
}



void RaportCreator::send_to_archive(Raport* rap) {
	arch_link->get(rap);
}

