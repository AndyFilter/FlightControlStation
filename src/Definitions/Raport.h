#include <exception>
#include <string>
#include <fstream>
//using namespace std;

#ifndef __Raport_h__
#define __Raport_h__

// #include "Archive.h"
// #include "RaportCreator.h"

//class Archive;
//class RaportCreator;
//class Raport;


class Raport
{
	public:
		//Archive* unnamed_Archive;
		//RaportCreator* unnamed_RaportCreator;
		Raport(string spcs);

		virtual void print(ofstream& output) = 0;
	protected:
		std::string plane_specs;

};

#endif
