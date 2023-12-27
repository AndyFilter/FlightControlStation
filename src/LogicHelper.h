#pragma once

#include "Definitions.h"

#include "Definitions/Airport.h"
#include "Definitions/Plane.h"
#include "Definitions/FlightPlan.h"

#include <vector>

#define PLANE_SPEED_MULTI 5

#define NAMES_COUNT 9
const char* const NAMES[NAMES_COUNT] = { u8"Tomasz", u8"Robert", u8"Andrzej", u8"Bogusław", u8"Antek", u8"Janusz", u8"Paweł" , u8"Bolesław" , u8"Jarek" };
const char* const SURNAMES[NAMES_COUNT] = { u8"Włodarski", u8"Andrzejewski", u8"Działowy", u8"Janowski", u8"Szostak", u8"Biały", u8"Krawczyk" , u8"Kowalski" , u8"Lewandowski" };

struct Message {
	std::string sender;
	std::string message;
};

inline std::vector<Message> messages;

// Functional Data
inline std::vector<Plane*> planes;
inline std::vector<Airport*> airports;
inline std::vector<FlightPlan*> flightplans;
inline Plane* selectedPlane = nullptr;
inline int selectedAirport = -1;
inline int selectedPlan = -1;
//inline std::vector<Pilot*> pilots;

// delta time passed as milliseconds
void UpdatePlanesPositions(float deltaTime);

// Do Random events etc.
void LogicTick(float deltaTime);

// Allocates memory for FlightPlan and Plane. Requires at least 2 airports
void GenerateFlight(Airport* start = nullptr, Airport* end = nullptr);