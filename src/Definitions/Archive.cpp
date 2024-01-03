#include <exception>
using namespace std;

#include "Archive.h"
#include "Raport.h"
#include "RaportCreator.h"

void Archive::set_data_output(string s) { //zmien w diag
	data_output.open(s+".txt");
}

void Archive::print() {	
	for (auto x : raportVector)
		x->print(data_output);

	data_output.close();
}

void Archive::get(Raport* rap) {	//zmien w diag
	raportVector.push_back(rap);
}

