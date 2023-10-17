#include "Gui/gui.h"
#include "Gui/GUI_Helper.h"
#include "Definitions.h"
#include "LogicHelper.h"

#include <vector>
#include <chrono>

inline uint64_t micros() {
	static auto programStart{ std::chrono::steady_clock::now() };
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - programStart).count();
}

// Program Internal Data
static bool hasExited = false;
uint64_t lastFrameRenderTime = 0; // in microseconds

// UI Controls
bool isRadarStopped = false;

// Functional Data
static std::vector<Plane> planes;
static std::vector<Airport> airports;
static int selectedPlane = -1;
static int selectedAirport = -1;

int OnGui()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	float dT = lastFrameRenderTime / 1000.f;

	DrawRadar(planes, airports, dT * !isRadarStopped, &selectedPlane, &selectedAirport);

	ImGui::SameLine();

	auto avail = ImGui::GetContentRegionAvail();

	ImGui::BeginGroup();
	//if (ImGui::Button("Stop the radar"))
	//	isRadarStopped = !isRadarStopped;

	ImGui::BeginGroup();

	//ImGui::SetNextItemWidth((avail.x - style.ItemSpacing.x) / 2);
	ImGui::Text("Samoloty");
	if (ImGui::BeginChild("##PlanesTable", { (avail.x - style.ItemSpacing.x) / 2, 200}, true))
	{
		//ImGui::ListBox("##PlanesTable", &selectedPlane, [](void* data, int idx) { return (const char*)planes[idx].identifier; }, planes.data(), (int)planes.size());
		for (int i = 0; i < planes.size(); i++)
		{
			if (ImGui::Selectable(planes[i].identifier, selectedPlane == i)) {
				if (selectedPlane == i)
					selectedPlane = -1;
				else {
					planes[i].isSelected = true;
					selectedPlane = i;
				}
			}

			if (ImGui::IsItemHovered())
				planes[i].isHovered = true;
			else
				planes[i].isHovered = false;

			if(selectedPlane != i)
				planes[i].isSelected = false;
		}
	}
	ImGui::EndChild();

	ImGui::Text("Lotniska");
	if (ImGui::BeginChild("##AirportsTable", { (avail.x - style.ItemSpacing.x) / 2, 200 }, true))
	{
		for (int i = 0; i < airports.size(); i++)
		{
			if (ImGui::Selectable(airports[i].identifier, selectedAirport == i)) {
				if (selectedAirport == i)
					selectedAirport = -1;
				else {
					airports[i].isSelected = true;
					selectedAirport = i;
				}
			}

			if (ImGui::IsItemHovered())
				airports[i].isHovered = true;
			else
				airports[i].isHovered = false;

			if (selectedAirport != i)
				airports[i].isSelected = false;
		}
	}
	ImGui::EndChild();

	ImGui::EndGroup(); 
	ImGui::SameLine();

	ImGui::BeginGroup();

	ImGui::Text("Informacje Lotu");
	if (ImGui::BeginChild("##FlightInfoPanel", { (avail.x - style.ItemSpacing.x) / 2, 200 }, true))
	{
		ImGui::Text("bla bla bla\ncos tu sie da :3");
	}
	ImGui::EndChild();

	
	ImGui::EndGroup();
	ImGui::EndGroup();


	return 0;
}

inline bool OnExit() { hasExited = true; return 1; }

int main()
{
	GUI::Setup(OnGui);
	GUI::onExitFunc = OnExit;

	//planes.push_back(Plane({ 0.2, 0.8 }, {0.01, -0.006}));
	planes.push_back(Plane({ 0.62, 0.63 }, {-0.008, 0.002}, "Plane001"));
	planes.push_back(Plane({ 0.2, 0.2 }, {-0.001, 0.006}, "Plane002"));

	airports.push_back(Airport({ 0.6, 0.3 }, "AP-001"));
	airports.push_back(Airport({ 0.8, 0.83 }, "AP-002"));

	auto frameStartRender = micros();
		
	while (!hasExited)
	{
		UpdatePlanesPositions(planes, lastFrameRenderTime / 1000.f);

		if (GUI::DrawGui())
			break;

		auto now = micros();
		lastFrameRenderTime = now - frameStartRender;
		frameStartRender = now;
		//printf("frame rendered in: %lldus\n", lastFrameRenderTime);
	}

	return 0;
}