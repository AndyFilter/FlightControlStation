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
				selectedPlane = nullptr;
			//if (i < selectedPlane)
			//	selectedPlane--;
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

Plane* GetRandomFreePlane(Airport* src = nullptr, Airport* dst = nullptr)
{
	if (src) {
		if (dst)
			return new Plane(src->position, (dst->position - src->position) / 1000 * ((rand() % 100) / 100.f + 1) * PLANE_SPEED_MULTI);
		return new Plane(src->position);
	}
	else
		return new Plane(Vec2(0.5, 0.3));
}

Airport* GetRandomAirport(Airport* ap = nullptr)
{
	if(!ap)
		return airports[rand() % airports.size()];

	Airport* dst;
	do
	{
		dst = GetRandomAirport();
	} while (dst == ap);

	return dst;

	//for (Airport*& a : airports) {
	//	if (a != ap)
	//		return a;
	//}
}

std::vector<Airport*> GetEmgAirports(Airport* src, Airport* dst, int count = 2) {
	std::vector<Airport*> aps;
	for (Airport*& a : airports) {
		if (a != src && a != dst)
			aps.push_back(a);

		if (aps.size() >= count)
			return aps;
	}

	return aps;
}

void LogicTick(float deltaTime)
{
	if (AcceptanceSystem::Instance()->queued < 2 && rand() % (100 * (planes.size()+1)) == 0) {
		// Fake pilot submitting a flight plan
		void GenerateFlight(Airport * start, Airport * end);
	}
}

void GenerateFlight(Airport* start, Airport* end)
{
	// ---------- OLD CODE ----------
	/*
	if (airports.size() < 2)
		return nullptr;

	if (!start)
		start = airports[rand() % airports.size()];

	while(!end || end == start)
		end = airports[rand() % airports.size()];

	Plane* plane = new Plane(start->position, (end->position-start->position)/1000 * ((rand() % 100) / 100.f + 1) * PLANE_SPEED_MULTI);
	plane->flightPlan = new FlightPlan(start, end);

	return plane;
	*/


	// ---------- NEW CODE ----------

	auto srcAirport = GetRandomAirport();
	auto dstAirport = GetRandomAirport(srcAirport);

	AcceptanceSystem::Instance()->submitFlightPlan(GetRandomFreePilot(), GetRandomFreePilot(), GetRandomFreePlane(srcAirport, dstAirport), srcAirport, dstAirport, GetEmgAirports(srcAirport, dstAirport));
}
