#pragma once
#include "../Definitions.h"

#ifndef __Airport_h__
#define __Airport_h__

// #include "FlightPlan.h"

//class FlightPlan;

class Airport
{
	public:
		//FlightPlan* unnamed_FlightPlan;
		Vec2 position;
		Vec2 _spottedPos = { -100, -100 };

		char identifier[64]{ "AP-001" }; // Unique plane identifier (functionality can depend on this being unique)

		bool isSelected = false; // UI thing
		bool isHovered = false; // UI thing

		long long id = 0; // unique from 0 to ...

		Airport(Vec2 pos) : position(pos) { id = count++; };
		Airport(Vec2 pos, const char name[64]) : position(pos) { id = count++; strcpy_s(identifier, name); };

	private:
		static long long count;
};

//class Airport {
//	static long long count;
//public:
//	Vec2 position;
//	Vec2 _spottedPos = { -100, -100 };
//
//	char identifier[64]{ "AP-001" }; // Unique plane identifier (functionality can depend on this being unique)
//
//	bool isSelected = false; // UI thing
//	bool isHovered = false; // UI thing
//
//	long long id = 0; // unique from 0 to ...
//
//	Airport(Vec2 pos) : position(pos) { id = count++; };
//	Airport(Vec2 pos, const char name[64]) : position(pos) { id = count++; strcpy_s(identifier, name); };
//};

#endif
