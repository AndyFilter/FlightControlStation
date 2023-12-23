#include "Gui/gui.h"
#include "Gui/GUI_Helper.h"
#include "Definitions.h"
#include "LogicHelper.h"

#include "Definitions/Airport.h"
#include "Definitions/Plane.h"
#include "Definitions/Pilot.h"
#include "Definitions/FlightPlan.h"
#include "Definitions/AcceptanceSystem.h"

#include <vector>
#include <chrono>
#include <time.h>

inline uint64_t micros() {
	static auto programStart{ std::chrono::steady_clock::now() };
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - programStart).count();
}

long long Plane::count = 1;
long long Airport::count = 0;

// Program Internal Data
static bool hasExited = false;
uint64_t lastFrameRenderTime = 0; // in microseconds

// UI Controls
bool isRadarStopped = false;

// MOVED TO LogicHelper.h
// Functional Data
//static std::vector<Plane*> planes;
//static std::vector<Airport*> airports;
//static int selectedPlane = -1;
//static int selectedAirport = -1;

int OnGui()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	float dT = lastFrameRenderTime / 1000.f;

	DrawRadar(planes, airports, dT * !isRadarStopped, &selectedPlane, &selectedAirport);

	ImGui::SameLine();

	auto avail = ImGui::GetContentRegionAvail();

	ImGui::BeginGroup();
	if (ImGui::Button("Stop the radar"))
		isRadarStopped = !isRadarStopped;
	if (ImGui::Button("Generate Flight")) {
		Plane* plane = GenerateFlight();
		planes.push_back(plane);
	}

	ImGui::BeginGroup();

	//ImGui::SetNextItemWidth((avail.x - style.ItemSpacing.x) / 2);
	ImGui::Text("Samoloty");
	if (ImGui::BeginChild("##PlanesTable", { (avail.x - style.ItemSpacing.x) / 2, 200}, true))
	{
		//ImGui::ListBox("##PlanesTable", &selectedPlane, [](void* data, int idx) { return (const char*)planes[idx].identifier; }, planes.data(), (int)planes.size());
		for (int i = 0; i < planes.size(); i++)
		{
			if (ImGui::Selectable(planes[i]->identifier, selectedPlane == i)) {
				if (selectedPlane == i)
					selectedPlane = -1;
				else {
					planes[i]->isSelected = true;
					selectedPlane = i;
				}
			}

			if (ImGui::IsItemHovered())
				planes[i]->isHovered = true;
			else
				planes[i]->isHovered = false;

			if(selectedPlane != i)
				planes[i]->isSelected = false;
		}
	}
	ImGui::EndChild();

	ImGui::Text("Lotniska");
	if (ImGui::BeginChild("##AirportsTable", { (avail.x - style.ItemSpacing.x) / 2, 200 }, true))
	{
		for (int i = 0; i < airports.size(); i++)
		{
			if (ImGui::Selectable(airports[i]->identifier, selectedAirport == i)) {
				if (selectedAirport == i)
					selectedAirport = -1;
				else {
					airports[i]->isSelected = true;
					selectedAirport = i;
				}
			}

			if (ImGui::IsItemHovered())
				airports[i]->isHovered = true;
			else
				airports[i]->isHovered = false;

			if (selectedAirport != i)
				airports[i]->isSelected = false;
		}
	}
	ImGui::EndChild();

	ImGui::EndGroup(); 
	ImGui::SameLine();

	ImGui::BeginGroup();

	ImGui::Text("Informacje o Locie");
	if (ImGui::BeginChild("##FlightInfoPanel", { (avail.x - style.ItemSpacing.x) / 2, 200 }, true))
	{
		if (selectedPlane != -1) {
			auto fp = planes[selectedPlane]->flightPlan;
			ImGui::Text("Z:");
			ImGui::SameLine();
			if(ImGui::Selectable(fp->startAirport->identifier, fp->startAirport->isSelected, 0, {ImGui::CalcTextSize(fp->startAirport->identifier).x,0})) {
				selectedAirport = fp->startAirport->id;
				fp->startAirport->isSelected = true;
			}
			ImGui::Text("Do:");
			ImGui::SameLine();
			if(ImGui::Selectable(fp->endAirport->identifier, fp->endAirport->isSelected, 0, {ImGui::CalcTextSize(fp->endAirport->identifier).x,0})) {
				selectedAirport = fp->endAirport->id;
				fp->endAirport->isSelected = true;
			}
		}
	}
	ImGui::EndChild();
	
	ImGui::Text("Akcje");
	if (ImGui::BeginChild("##FlightActionPanel", { (avail.x - style.ItemSpacing.x) / 2, 200 }, true))
	{
	}
	ImGui::EndChild();

	
	ImGui::EndGroup();

	ImGui::Text("Logi");
	if (ImGui::BeginChild("##Logs", { -1, -1 }, true))
	{
	}
	ImGui::EndChild();

	ImGui::EndGroup();


	return 0;
}

inline bool OnExit() { hasExited = true; return 1; }

int main()
{
	srand(time(NULL));

	GUI::Setup(OnGui);
	GUI::onExitFunc = OnExit;

	ImGui::GetIO().IniFilename = nullptr;

	//planes.push_back(Plane({ 0.2, 0.8 }, {0.01, -0.006}));
	//planes.push_back(new Plane({ 0.62, 0.63 }, {-0.008, 0.002}, "Plane001"));
	//planes.push_back(new Plane({ 0.2, 0.2 }, {-0.001, 0.006}, "Plane002"));

	airports.push_back(new Airport({ 0.6, 0.3 }, "AP-001"));
	airports.push_back(new Airport({ 0.8, 0.83 }, "AP-002"));
	airports.push_back(new Airport({ 0.2, 0.76 }, "AP-003"));
	airports.push_back(new Airport({ 0.25, 0.15 }, "AP-004"));

	//pilots.push_back(new Pilot("Adam", "Bialy", 32));
	//pilots.push_back(new Pilot("Robert", "Xd", 12));
	//pilots.push_back(new Pilot("Tomasz", "Dzialowy", 44));

	auto frameStartRender = micros();
		
	while (!hasExited)
	{
		float dt = lastFrameRenderTime / 1000.f * !isRadarStopped;
		UpdatePlanesPositions(dt);

		LogicTick(dt);

		if (GUI::DrawGui())
			break;

		auto now = micros();
		lastFrameRenderTime = now - frameStartRender;
		frameStartRender = now;
		//printf("frame rendered in: %lldus\n", lastFrameRenderTime);
	}

	return 0;
}