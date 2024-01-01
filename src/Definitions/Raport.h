#include <exception>
#include <string>
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

		virtual void print() = 0;
	private:
		std::string plane_specs;

};

#endif
