#include <exception>
#include <vector>

//using namespace std;

#ifndef __Archive_h__
#define __Archive_h__

#include "Raport.h"
// #include "RaportCreator.h"

class Raport;
class RaportCreator;
//class Archive;

class Archive
{
	public:
		//RaportCreator* unnamed_RaportCreator;
		//Raport* unnamed_Raport;

		void print(unsigned index);
		void find(std::string string);
	private:
		std::vector<Raport> raportVector;

};

#endif
