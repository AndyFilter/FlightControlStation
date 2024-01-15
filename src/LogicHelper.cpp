#include "LogicHelper.h"

#include "Definitions/AcceptanceSystem.h"
#include "Definitions/Pilot.h"

#define sign(x) (x == 0 ? 0 : x < 0 ? -1 : 1)

void PlaneLanded(Plane* plane) {
	if (plane->isSelected)
		selectedPlane = nullptr;
	plane->isAirborn = false;
	RCreator.create_raport(*(plane->flightPlan));
}

// Euler velocity/position method
void UpdatePlanesPositions(float deltaTime)
{
	for (int i = 0; i < planes.size(); i++)
	{
		if (!planes[i]->isAirborn)
			continue;

		planes[i]->position += planes[i]->velocity * deltaTime/1000;

		if (sign((planes[i]->flightPlan->endAirport->position-planes[i]->position).x) != sign(planes[i]->velocity.x)) {
			PlaneLanded(planes[i]);
		}
	}
}

Pilot* GetRandomFreePilot()
{
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

Plane* GetRandomAirbornPlane() {
	Plane* plane = planes[rand() % planes.size()];

	int iter = 0;

	while (!plane->isAirborn && iter < 200)
		plane = planes[rand() % planes.size()];

	

	return plane;
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

void SetPlaneBadPath(Plane* plane) {
	plane->isBadDirection = true;
}

void SetPlaneMayday(Plane* p) {
	messages.push_back(Message(p->identifier, "MAYDAY, MAYDAY, MAYDAY. " + p->flightPlan->pilots[0]->lastName + ". Lot z " + p->flightPlan->startAirport->identifier + ". OVER"));

	if (selectedPlan = p->flightPlan) {
		selectedPlan = nullptr;
		selectedPlane = nullptr;
	}
	ES.notify_emergency_services(*(p->flightPlan));

	flightplans.erase(std::find(flightplans.begin(), flightplans.end(), p->flightPlan));
	delete p->flightPlan;
	planes.erase(std::find(planes.begin(), planes.end(), p));
	delete p;
}

void LogicTick(float deltaTime)
{
	float fixedRand = deltaTime * rand();

	if (AcceptanceSystem::Instance()->queued_count < 3 && fmodf(fixedRand, (150 * (planes.size()+1))) < 1) {
		// Fake pilot submitting a flight plan
		GenerateFlight();
	}

	if (!planes.empty() && fmodf(fixedRand, 1500) < 1) {
		// Fake pilot falling asleep or smth ¯\_(ツ)_/¯
		SetPlaneBadPath(GetRandomAirbornPlane());
	}

	if (!planes.empty() && fmodf(fixedRand, 10000) < 1) {
		// MAY-DAY signal (destroys the plane and the flightplan)
		Plane* p = GetRandomAirbornPlane();
		SetPlaneMayday(p);
	}

	for (Plane*& p : planes) {
		if (p->wasBadSpotted)
			p->badTimer += deltaTime;

		// Plane can be up to 20 seconds in the "bad" state
		if (p->badTimer > 20000) {
			SetPlaneMayday(p);
		}
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
	
	// Used to avoid creating 2 flights from the same airport one after another
	static Airport* last_chosen_airport{ nullptr };

	auto srcAirport = GetRandomAirport(last_chosen_airport);
	auto dstAirport = GetRandomAirport(srcAirport);

	last_chosen_airport = srcAirport;

	AcceptanceSystem::Instance()->submitFlightPlan(GetRandomFreePilot(), GetRandomFreePilot(), GetRandomFreePlane(srcAirport, dstAirport), srcAirport, dstAirport, GetEmgAirports(srcAirport, dstAirport));
}
