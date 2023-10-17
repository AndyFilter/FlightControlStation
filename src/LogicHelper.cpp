#include "LogicHelper.h"

void UpdatePlanesPositions(std::vector<Plane>& planes, float deltaTime)
{
	for (int i = 0; i < planes.size(); i++)
	{
		planes[i].position += planes[i].velocity * deltaTime/1000;
	}
}
