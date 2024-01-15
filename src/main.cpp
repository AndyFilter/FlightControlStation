#include "Gui/gui.h"
#include "Gui/GUI_Helper.h"
#include "Definitions.h"
#include "LogicHelper.h"

#include "Definitions/Airport.h"
#include "Definitions/Plane.h"
#include "Definitions/Pilot.h"
#include "Definitions/FlightPlan.h"
#include "Definitions/AcceptanceSystem.h"
#include "Definitions/RaportCreator.h"


#include <vector>
#include <chrono>
#include <time.h>


inline uint64_t micros() {
	static auto programStart{ std::chrono::steady_clock::now() };
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - programStart).count();
}

long long Plane::count = 1;
long long Airport::count = 0;
long Pilot::count = 0;

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

	DrawRadar(planes, airports, dT * !isRadarStopped, &selectedAirport);

	ImGui::SameLine();

	auto avail = ImGui::GetContentRegionAvail();

	ImGui::BeginGroup();
#ifdef _DEBUG
	if (ImGui::Button("Generate Flight")) {
		GenerateFlight();
		//Plane* plane = GenerateFlight();
		//planes.push_back(plane);
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop the radar"))
		isRadarStopped = !isRadarStopped;
	#endif
	ImGui::BeginGroup();


	//ImGui::SetNextItemWidth((avail.x - style.ItemSpacing.x) / 2);
#pragma region SAMOLOTY
	/*
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
	*/
#pragma endregion SAMOLOTY

	// Menu panel with flight plans
	ImGui::Text("Loty");
	if (ImGui::BeginChild("##FlightsTable", { (avail.x - style.ItemSpacing.x) / 2, 200}, true))
	{
		//ImGui::ListBox("##PlanesTable", &selectedPlane, [](void* data, int idx) { return (const char*)planes[idx].identifier; }, planes.data(), (int)planes.size());
		for (int i = 0; i < flightplans.size(); i++)
		{
			if (!flightplans[i]->plane->isAirborn)
				continue;

			char label[24];
			sprintf_s(label, "Plan%03d", i + 1);
			if (ImGui::Selectable(label, selectedPlan == flightplans[i])) {
				if (selectedPlan == flightplans[i])
					selectedPlan = nullptr;
				else {
					//flightplans[i]->plane->isSelected = true;
					selectedPlan = flightplans[i];
				}
			}

			//if (ImGui::IsItemHovered())
			//	flightplans[i]->plane->isHovered = true;
			//else
			//	flightplans[i]->plane->isHovered = false;

			if (selectedPlan != flightplans[i]) {
				flightplans[i]->plane->isSelected = false;
			}
		}
	}

	ImGui::EndChild();

	// Menu panel with airports
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
		if (selectedPlan) {
			auto fp = selectedPlan;
			ImGui::Text("Z:");
			ImGui::SameLine();
			//if(ImGui::Selectable(fp->startAirport->identifier, fp->startAirport->isSelected, 0, {ImGui::CalcTextSize(fp->startAirport->identifier).x,0})) {
			//	selectedAirport = fp->startAirport->id;
			//	fp->startAirport->isSelected = true;
			//}
			if(AirportText(fp->startAirport))
				selectedAirport = fp->startAirport->id;
			ImGui::Text("Do:");
			ImGui::SameLine();
			if (AirportText(fp->endAirport))
				selectedAirport = fp->endAirport->id;
			//if(ImGui::Selectable(fp->endAirport->identifier, fp->endAirport->isSelected, 0, {ImGui::CalcTextSize(fp->endAirport->identifier).x,0})) {
			//	selectedAirport = fp->endAirport->id;
			//	fp->endAirport->isSelected = true;
			//}

			for (int i = 0; i < fp->pilots.size(); i++) {
				ImGui::Text(u8"Pilot %i: %s %s", i+1, fp->pilots[i]->firstName.c_str(), fp->pilots[i]->lastName.c_str());
			}

			ImGui::Text("Samolot:"); ImGui::SameLine();
			if (ImGui::Selectable(fp->plane->identifier, fp->plane->isSelected, 0, { ImGui::CalcTextSize(fp->plane->identifier).x,0})) {
				selectedPlane = fp->plane->isSelected ? nullptr : fp->plane;
				fp->plane->isSelected ^= true;
			}

			//if (ImGui::IsItemHovered())
			//	flightplans[i]->plane->isHovered = true;
			//else
			//	flightplans[i]->plane->isHovered = false;

			//if (selectedPlane != i)
			//	flightplans[i]->plane->isSelected = false;
		}
	}
	ImGui::EndChild();
	
	ImGui::Text("Akcje");
	if (ImGui::BeginChild("##FlightActionPanel", { (avail.x - style.ItemSpacing.x) / 2, 200 }, true))
	{
		if (selectedPlane) {
			ImGui::BeginDisabled(!selectedPlane->wasBadSpotted);
			{
				if (ImGui::Button(u8"Poinformuj pilota o kierunku", {-1, 0})) {
					selectedPlane->CorrectFlight();
				}
			}
			ImGui::EndDisabled();

#ifdef _DEBUG
			//if (ImGui::Button(u8"Poświeć Pilotom laserem w oczy")) {
			//	flightplans.erase(std::find(flightplans.begin(), flightplans.end(), selectedPlane->flightPlan));
			//	delete selectedPlane->flightPlan;
			//	Plane* plane = selectedPlane;
			//	planes.erase(std::find(planes.begin(), planes.end(), selectedPlane));
			//	delete plane;
			//	selectedPlan = nullptr;
			//	selectedPlane = nullptr;
			//}
			//ImGui::SetItemTooltip(u8"Może spowodować nieoczekiwane uszczerbki na zdrowiu*");
#endif
		}

	}
	ImGui::EndChild();

	
	ImGui::EndGroup();

	ImGui::Text("Logi / Plany Lotu (%i)", AcceptanceSystem::Instance()->queued_count);
	if (ImGui::BeginChild("##Logs", { -1, -1 }, true))
	{
		auto fps = AcceptanceSystem::Instance()->q_flightplans; // flight plans
		if (AcceptanceSystem::Instance()->queued_count) {
			//for (int i = 0; i < fps.size(); i++) {
			//	auto fp = fps.;
			//	ImGui::TextWrapped("[Plan Lotu] - ")
			//}
			auto fp = fps.front();

			ImGui::Text("[Plan Lotu]:"); ImGui::SameLine();
			if (AirportText(fp->startAirport)) 
				selectedAirport = fp->startAirport->id;

			ImGui::SameLine(); ImGui::Text("->"); ImGui::SameLine();
			if (AirportText(fp->endAirport))
				selectedAirport = fp->endAirport->id;
			ImGui::SameLine();

			ImGui::Text(u8"| Samolot: %s", fp->plane->identifier);

			if(fp->pilots.size() > 1)
				ImGui::Text(u8"(Piloci: 1: %c. %s, 2: %c. %s)", fp->pilots[0]->firstName[0], fp->pilots[0]->lastName.c_str(), fp->pilots[1]->firstName[0], fp->pilots[1]->lastName.c_str());
			ImGui::SameLine();
			if (ImGui::Button(u8"Zatwierdź")) {
				AcceptanceSystem::Instance()->verify(true);
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"Odrzuć")) {
				AcceptanceSystem::Instance()->verify(false);
			}

			ImGui::Separator();
		}

		for (Message& m : messages) {
			ImGui::TextWrapped("[%s]: %s", m.sender.c_str(), m.message.c_str());
		}
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

	assert(airports.size() >= 4);
	assert(ImGui::GetIO().Ctx); // Make sure the UI was set-up correctly

	/*04.01.2024 KHan*/
	Arch.set_data_output("test");
	RCreator.set_Archive_link(&Arch);
		
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

	Arch.print();


	return 0;
}