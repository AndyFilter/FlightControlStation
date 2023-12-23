#pragma once

#include "Definitions.h"

#include "Definitions/Airport.h"
#include "Definitions/Plane.h"
#include "Definitions/FlightPlan.h"

#include <vector>

#define NAMES_COUNT 6
const char* const NAMES[NAMES_COUNT] = {"Tomasz", "Robert", "Andrzej", "Bogusław", "Amelia", "Janusz"};
const char* const SURNAMES[NAMES_COUNT] = {"Włodarski", "Andrzejewski", "Działowy", "Janowski", "Szostak", "Biały"};

// Functional Data
inline std::vector<Plane*> planes;
inline std::vector<Airport*> airports;
inline int selectedPlane = -1;
inline int selectedAirport = -1;
inline std::vector<Pilot*> pilots;

// delta time passed as milliseconds
void UpdatePlanesPositions(float deltaTime);

// Do Random events etc.
void LogicTick(float deltaTime);

// Allocates memory for FlightPlan and Plane. Requires at least 2 airports
Plane* GenerateFlight(Airport* start = nullptr, Airport* end = nullptr);