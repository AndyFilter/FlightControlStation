#pragma once

#include <exception>
#include <string>
#include "../Definitions.h"

#include <stdio.h>
#include <time.h>
#include <random>

//using namespace std;

//#ifndef __Plane_h__
//#define __Plane_h__

// #include "FlightPlan.h"

class FlightPlan;

class Plane
{
	public:
		//FlightPlan* unnamed_FlightPlan;

		void change_velocity(float mult);
		void set_altitude(std::string x__int);
		float* get_pos();
		float get_alt();
		float get_velocity();

	private:
		static long long count;
		long long id = 0;
		
	public:
		Vec2 position{ 0,0 }; // relative position [0,1]
		Vec2 _spottedPos{ 0,0 }; // Used for displaying on the radar 
		Vec2 velocity{ 0,0 }; // Euler method, units same as position / t 
		float radarAge = 0; // in millis

		float altitude;
		std::string specifications;

		char identifier[64]{ "Plane001" }; // Unique plane identifier (functionality should not depend on this being unique)
		FlightPlan* flightPlan = nullptr;

		bool isSelected = false; // UI thing
		bool isHovered = false; // UI thing

		Plane(Vec2 pos) : position(pos) { id = count++; GeneratePlaneIdentifier(); };
		Plane(Vec2 pos, const char name[64]) : position(pos) { id = count++; strcpy_s(identifier, name); };
		Plane(Vec2 pos, Vec2 velocity) : position(pos), velocity(velocity) { id = count++; GeneratePlaneIdentifier(); };
		Plane(Vec2 pos, Vec2 velocity, const char name[64]) : position(pos), velocity(velocity) { id = count++; strcpy_s(identifier, name); };

	private:
		inline void GeneratePlaneIdentifier() {
			srand(time(NULL) * id);
			sprintf_s<64>(identifier, "Plane-%03d", rand() % 1000);
		}

};


// Old Class

//class FlightPlan;
//
//class Plane {
//	static long long count;
//	long long id = 0;
//public:
//	Vec2 position; // relative position [0,1]
//	Vec2 _spottedPos; // Used for displaying on the radar 
//	Vec2 velocity; // Euler method, units same as position / t 
//	float radarAge = 0; // in millis
//
//	char identifier[64]{ "Plane001" }; // Unique plane identifier (functionality should not depend on this being unique)
//	FlightPlan* flightPlan = nullptr;
//
//	bool isSelected = false; // UI thing
//	bool isHovered = false; // UI thing
//
//	Plane(Vec2 pos) : position(pos) { id = count++; GeneratePlaneIdentifier(); };
//	Plane(Vec2 pos, const char name[64]) : position(pos) { id = count++; strcpy_s(identifier, name); };
//	Plane(Vec2 pos, Vec2 velocity) : position(pos), velocity(velocity) { id = count++; GeneratePlaneIdentifier(); };
//	Plane(Vec2 pos, Vec2 velocity, const char name[64]) : position(pos), velocity(velocity) { id = count++; strcpy_s(identifier, name); };
//private:
//	void GeneratePlaneIdentifier() {
//		srand(time(NULL) * id);
//		sprintf_s<64>(identifier, "Plane-%03d", rand() % 1000);
//	}
//};

//#endif
