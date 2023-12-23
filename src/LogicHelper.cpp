#include "LogicHelper.h"

#include "Definitions/AcceptanceSystem.h"
#include "Definitions/Pilot.h"

#define sign(x) (x == 0 ? 0 : x < 0 ? -1 : 1)

// Euler velocity/position method
void UpdatePlanesPositions(float deltaTime)
{
	for (int i = 0; i < planes.size(); i++)
	{
		planes[i]->position += planes[i]->velocity * deltaTime/1000;

		if (sign((planes[i]->flightPlan->endAirport->position-planes[i]->position).x) != sign(planes[i]->velocity.x)) {
			if (planes[i]->isSelected)
				selectedPlane = -1;
			if (i < selectedPlane)
				selectedPlane--;
			delete planes[i];
			planes.erase(planes.begin() + i);
			i--;
		}
	}
}

Pilot* GetRandomFreePilot()
{
	// Albo wybrać z listy pilotów, albo..
	//for (Pilot*& pilot : pilots)
	//{
	//}
	// Albo wygenerować nowego
	return new Pilot(NAMES[rand() % NAMES_COUNT], SURNAMES[rand() % NAMES_COUNT], (rand() % 30) + 24);
}

Plane* GetRandomFreePlane(Airport* ap = nullptr)
{
	if (ap)
		return new Plane(ap->position);
	else
		return new Plane(Vec2(0.5, 0.3));
}

Airport* GetRandomAirport()
{
	return airports[rand() % airports.size()];
}

void LogicTick(float deltaTime)
{
	if (!AcceptanceSystem::Instance()->queued && rand() % 100 == 0) {
		// Fake pilot submitting a flight plan
		auto srcAirport = GetRandomAirport();
		AcceptanceSystem::Instance()->submitFlightPlan(GetRandomFreePilot(), GetRandomFreePlane(srcAirport), srcAirport, GetRandomAirport());
	}
}

Plane* GenerateFlight(Airport* start, Airport* end)
{
	if (airports.size() < 2)
		return nullptr;

	if (!start)
		start = airports[rand() % airports.size()];

	while(!end || end == start)
		end = airports[rand() % airports.size()];

	Plane* plane = new Plane(start->position, (end->position-start->position)/100 * ((rand() % 100) / 100.f + 1));
	plane->flightPlan = new FlightPlan(start, end);

	return plane;
}
