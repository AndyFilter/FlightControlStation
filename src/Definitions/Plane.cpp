#include <exception>
#include <string>
using namespace std;

#include "Plane.h"
#include "FlightPlan.h"

void Plane::change_velocity(float mult) {
	velocity = velocity * mult;
}

void Plane::set_altitude(string x__int) {
	return;
}

Vec2 Plane::get_pos() {
	return position;
}

float Plane::get_alt() {
	return 1700 + (rand() % 500);
}

Vec2 Plane::get_velocity() {
	return velocity;
}

void Plane::CorrectFlight()
{
	isBadDirection = false;
	badTimer = 0;
}

