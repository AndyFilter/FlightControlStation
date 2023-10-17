#pragma once

#include "../Definitions.h"

#include <vector>

#define RADAR_SIZE 550 // Only divisible by 50
#define RADAR_SIZE_MARGIN 100 // Only divisible by 50*2
#define RADAR_OUTER_SIZE (RADAR_SIZE_MARGIN+RADAR_SIZE)
#define RADAR_CIRCLES 6
#define RADAR_BG_COLOR ImVec4(0.05, 0.1, 0.05, 1)
#define RADAR_CIRCLE_COLOR ImColor(0.05f, 0.9f, 0.05f, 0.72)
#define RADAR_PLANE_COLOR ImColor(0.1f, 0.95f, 0.1f, 1.f)
#define RADAR_TEXT_COLOR 0.3f, 1.f, 0.3f
#define RADAR_SELECT_MARK_COLOR 0.1f, 0.95f, 0.1f
#define RADAR_SELECT_MARK_SIZE 15
#define RADAR_SELECT_MARK_GAP 5
#define RADAR_ANGULAR_SPEED 3
#define RADAR_AIRPORT_SIZE 15

// Default size is RADAR_SIZExRADAR_SIZE;
void DrawRadar(std::vector<Plane>& planes, std::vector<Airport>& airports, float deltaTime, \
	int* selectedPlane = nullptr, int* selectedAirport = nullptr);