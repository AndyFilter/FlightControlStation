#pragma once
#include "External/ImGui/imgui.h"
//#include <math.h>
//#include <stdio.h>
//#include <time.h>
#include <random>

// And declarations :33 (i'm lazy)

#define PLANE_RADAR_MAX_AGE 2000

struct Vec2
{
public:
	float x, y; // x - horizontal, y - vertical

	Vec2(float x = 0, float y = 0) : x(x), y(y) {};

	inline const float dist(const Vec2& other) { return sqrt(powf(x - other.x, 2) + powf(y - other.y, 2)); };
	inline const float dist(const ImVec2& other) { return sqrt(powf(x - other.x, 2) + powf(y - other.y, 2)); };

	Vec2 operator=(const Vec2& other) { return { this->x = other.x, this->y = other.y }; }
	Vec2 operator+(const Vec2& other) { return { this->x + other.x, this->y + other.y }; }
	Vec2 operator-(const Vec2& other) { return { this->x - other.x, this->y - other.y }; }
	Vec2 operator+=(const Vec2& other) { return { this->x += other.x, this->y += other.y }; }
	Vec2 operator*(const float scale) { return { this->x * scale, this->y * scale }; }
	Vec2 operator/(const float scale) { return { this->x / scale, this->y / scale }; }
	Vec2 operator-(const float offset) { return { this->x - offset, this->y - offset }; }
	Vec2 operator+(const float offset) { return { this->x + offset, this->y + offset }; }
	ImVec2 operator+(const ImVec2& other) { return { this->x + other.x, this->y + other.y }; }
	ImVec2 operator-(const ImVec2& other) { return { this->x - other.x, this->y - other.y }; }

	operator ImVec2() const { return { x,y }; }
};

//class Airport;
//
//class FlightPlan {
//public:
//	Airport* startAirport = nullptr;
//	Airport* endAirport = nullptr;
//
//public:
//
//	FlightPlan(Airport* startAirport = nullptr, Airport* endAirport = nullptr)
//		: startAirport(startAirport), endAirport(endAirport)
//	{
//	}
//};

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
//		sprintf_s<64>(identifier, "Plane-%03d", rand()%1000);
//	}
//};

//class Airport {
//	static long long count;
//public:
//	Vec2 position;
//	Vec2 _spottedPos = {-100, -100};
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
