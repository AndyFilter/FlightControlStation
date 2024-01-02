#include <exception>
#include <string>
using namespace std;

#include "Raport.h"
#include "Archive.h"
#include "RaportCreator.h"

Raport::Raport(string spcs) {
	plane_specs = spcs;
}

//PURE-VIRTUAL
//void Raport::print() {
//	throw "Not yet implemented";
//}

