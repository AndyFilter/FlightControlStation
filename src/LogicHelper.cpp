#include "LogicHelper.h"

#define sign(x) (x == 0 ? 0 : x < 0 ? -1 : 1)

void UpdatePlanesPositions(std::vector<Plane*>& planes, int &selectedPlane, float deltaTime)
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

Plane* GenerateFlight(std::vector<Airport*>& airports, Airport* start, Airport* end)
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
