#pragma once

#include "Definitions.h"

#include <vector>

// delta time passed as milliseconds
void UpdatePlanesPositions(std::vector<Plane*>& planes, int& selectedPlane, float deltaTime);

// Allocates memory for FlightPlan and Plane. Requires at least 2 airports
Plane* GenerateFlight(std::vector<Airport*>& airports, Airport* start = nullptr, Airport* end = nullptr);