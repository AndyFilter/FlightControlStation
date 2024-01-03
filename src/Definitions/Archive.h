#include <exception>
#include <vector>
#include <fstream>

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

		void set_data_output(string s);
		void print();		//zmien w diag
		void get(Raport* rap);//zmien w diag
	private:
		std::vector<Raport*> raportVector;
		ofstream data_output;

};

#endif
