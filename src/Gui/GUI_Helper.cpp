#include "GUI_Helper.h"
#include "../External/ImGui/imgui.h"
#include "../External/ImGui/imgui_internal.h"
#include "../Audio.h"

#include <math.h>
#include <stdio.h>
#include <vector>

#include <Windows.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")

#define RADAR_SIZE_DIV_2 RADAR_SIZE/2
#define RADAR_HALF_MARGIN (RADAR_SIZE_MARGIN / 2)
#define M_PI 3.141592653589793238463
#define M_PI_DIV_4 0.78539816339744830961575

inline Vec2 ToRadarPos(Vec2 relativePos) {
	return { relativePos.x * RADAR_SIZE, relativePos.y * RADAR_SIZE };
}

void DrawElementMark(ImVec2 pos, ImColor color, ImDrawList *dl, bool drawCross = true)
{
	dl->AddLine(pos + ImVec2(-RADAR_SELECT_MARK_SIZE, -RADAR_SELECT_MARK_SIZE), pos + ImVec2(-RADAR_SELECT_MARK_GAP, -RADAR_SELECT_MARK_SIZE), color);
	dl->AddLine(pos + ImVec2(-RADAR_SELECT_MARK_SIZE, -RADAR_SELECT_MARK_SIZE), pos + ImVec2(-RADAR_SELECT_MARK_SIZE, -RADAR_SELECT_MARK_GAP), color);

	dl->AddLine(pos + ImVec2(RADAR_SELECT_MARK_GAP, -RADAR_SELECT_MARK_SIZE), pos + ImVec2(RADAR_SELECT_MARK_SIZE, -RADAR_SELECT_MARK_SIZE), color);
	dl->AddLine(pos + ImVec2(RADAR_SELECT_MARK_SIZE, -RADAR_SELECT_MARK_SIZE), pos + ImVec2(RADAR_SELECT_MARK_SIZE, -RADAR_SELECT_MARK_GAP), color);

	dl->AddLine(pos + ImVec2(RADAR_SELECT_MARK_GAP, RADAR_SELECT_MARK_SIZE), pos + ImVec2(RADAR_SELECT_MARK_SIZE, RADAR_SELECT_MARK_SIZE), color);
	dl->AddLine(pos + ImVec2(RADAR_SELECT_MARK_SIZE, RADAR_SELECT_MARK_SIZE + 1), pos + ImVec2(RADAR_SELECT_MARK_SIZE, RADAR_SELECT_MARK_GAP), color);

	dl->AddLine(pos + ImVec2(-RADAR_SELECT_MARK_GAP, RADAR_SELECT_MARK_SIZE), pos + ImVec2(-RADAR_SELECT_MARK_SIZE, RADAR_SELECT_MARK_SIZE), color);
	dl->AddLine(pos + ImVec2(-RADAR_SELECT_MARK_SIZE, RADAR_SELECT_MARK_SIZE), pos + ImVec2(-RADAR_SELECT_MARK_SIZE, RADAR_SELECT_MARK_GAP), color);

	// Cross
	if (drawCross) {
		dl->AddLine(pos + ImVec2(0, -RADAR_SELECT_MARK_GAP), pos + ImVec2(0, RADAR_SELECT_MARK_GAP), color);
		dl->AddLine(pos + ImVec2(RADAR_SELECT_MARK_GAP, 0), pos + ImVec2(-RADAR_SELECT_MARK_GAP, 0), color);
	}
}

void DrawRadar(std::vector<Plane*>& planes, std::vector<Airport*>& airports, float deltaTime, int* selectedPlane, int* selectedAirport)
{
	static bool isRadarSetUp = false; // used for animations
	static int curCircles = 1; // used for animations
	static int frameIdx = -20; // used for animations

	auto beginPos = ImGui::GetCursorPos();
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::PushStyleColor(ImGuiCol_FrameBg, RADAR_BG_COLOR);
	ImGui::BeginChildFrame(ImGui::GetID("RadarFrame"), { RADAR_OUTER_SIZE,RADAR_OUTER_SIZE }, 0);
	ImDrawList* dl = ImGui::GetWindowDrawList();

	// Draw Grid 
	for (int x = 0; x <= RADAR_OUTER_SIZE; x += 50) {
		dl->AddLine({ (float)x + beginPos.x, beginPos.y }, { (float)x + beginPos.x, (float)RADAR_OUTER_SIZE + beginPos.y }, ImColor(255, 255, 255, 30), 1.5f);
	}
	for (int y = 0; y <= RADAR_OUTER_SIZE; y += 50) {
		dl->AddLine({ beginPos.x, (float)y + beginPos.y }, { (float)RADAR_OUTER_SIZE + beginPos.x, (float)y + beginPos.y }, ImColor(255, 255, 255, 30), 1.5f);
	}

	Vec2 basePos = { RADAR_SIZE_DIV_2 + beginPos.x + RADAR_HALF_MARGIN, RADAR_SIZE_DIV_2 + beginPos.y + RADAR_HALF_MARGIN };

	// The actual radar
	{
		int step = RADAR_SIZE_DIV_2 / (RADAR_CIRCLES-1);
		for (int i = 1; i < curCircles; i++)
		{
			dl->AddCircle(basePos, (i * step) - 2, RADAR_CIRCLE_COLOR, 64);
		}
		dl->AddCircleFilled(basePos, 5, RADAR_CIRCLE_COLOR, 64);
	}

	ImGui::EndChildFrame();
	ImGui::PopStyleColor();

	if (!isRadarSetUp && frameIdx++ > 0 && frameIdx % 10 == 0 && ++curCircles == RADAR_CIRCLES)
		isRadarSetUp = true;

	if (!isRadarSetUp)
		return;

	static float angularPos = 0;
	dl->AddLine(basePos, { ((RADAR_SIZE_DIV_2 - 2) * sinf(angularPos)) + basePos.x, ((RADAR_SIZE_DIV_2 - 2) * cosf(angularPos)) + basePos.y }, RADAR_CIRCLE_COLOR, 1.8f); // Imagine having a small monitor
	angularPos += 0.001 * RADAR_ANGULAR_SPEED * deltaTime;
	short scan_quarter = (short)floor(fmodf(angularPos, M_PI * 2) * 2 / M_PI);
	//printf("ang Pos = %f\n", fmodf(angularPos, M_PI));

	// Draw Spotted Planes / Spot Planes
	for (int i = 0; i < planes.size(); i++)
	{
#ifdef RADAR_FADING_PLANES
		if ((planes[i]->radarAge -= deltaTime) > 0) {
			if (planes[i]->radarAge - PLANE_RADAR_MAX_AGE > 0) // Wait delay used to sync visuals
				continue;
#else
		if (planes[i]->radarAge) {
#endif


			if (selectedPlane) {
				if ((planes[i]->_spottedPos + Vec2(beginPos.x + RADAR_HALF_MARGIN, beginPos.y + RADAR_HALF_MARGIN)).dist(ImGui::GetMousePos()) < RADAR_SELECT_MARK_SIZE)
				{
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
						*selectedPlane = planes[i]->isSelected ? -1 : i;
						planes[i]->isSelected ^= true;
					}
					planes[i]->isHovered = true;
				}
			}

#ifdef RADAR_FADING_PLANES
			float opacity = planes[i]->radarAge / PLANE_RADAR_MAX_AGE;
			if (planes[i]->isHovered)
				dl->AddCircleFilled(planes[i]->_spottedPos + Vec2(beginPos.x + RADAR_HALF_MARGIN, beginPos.y + RADAR_HALF_MARGIN), 10, ImColor(0.4f, 1.f, 0.4f, opacity));
			else
				dl->AddCircleFilled(planes[i]->_spottedPos + Vec2(beginPos.x + RADAR_HALF_MARGIN, beginPos.y + RADAR_HALF_MARGIN), 10, ImColor(0.1f, 0.95f, 0.1f, opacity));

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(RADAR_TEXT_COLOR, opacity));

			auto startPos = planes[i]->_spottedPos + RADAR_HALF_MARGIN + ImGui::WindowPosRelToAbs(ImGui::GetCurrentWindow(), { 0,0 });
			ImGui::RenderTextClipped({ startPos.x - 100, startPos.y - 50 }, { startPos.x + 100, startPos.y + 100 }, planes[i]->identifier, nullptr, nullptr, { 0.5, 0.5 });

			// Draw Rectangle Mark
			if (planes[i]->isSelected)
				DrawElementMark(startPos, ImColor(RADAR_SELECT_MARK_COLOR, opacity), dl, true);

			ImGui::PopStyleColor();

			continue;
		}
#else
			if (planes[i]->isHovered)
				dl->AddCircleFilled(planes[i]->_spottedPos + Vec2(beginPos.x + RADAR_HALF_MARGIN, beginPos.y + RADAR_HALF_MARGIN), 10, ImColor(0.4f, 1.f, 0.4f, 1.f));
			else
				dl->AddCircleFilled(planes[i]->_spottedPos + Vec2(beginPos.x + RADAR_HALF_MARGIN, beginPos.y + RADAR_HALF_MARGIN), 10, ImColor(0.1f, 0.95f, 0.1f, 1.f));

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(RADAR_TEXT_COLOR, 1.f));

			auto startPos = planes[i]->_spottedPos + RADAR_HALF_MARGIN + ImGui::WindowPosRelToAbs(ImGui::GetCurrentWindow(), { 0,0 });
			ImGui::RenderTextClipped({ startPos.x - 100, startPos.y - 50 }, { startPos.x + 100, startPos.y + 100 }, planes[i]->identifier, nullptr, nullptr, { 0.5, 0.5 });

			// Draw Rectangle Mark
			if (planes[i]->isSelected)
				DrawElementMark(startPos, ImColor(RADAR_SELECT_MARK_COLOR, 1.f), dl, true);

			ImGui::PopStyleColor();

			if ((planes[i]->radarAge -= deltaTime) > 500)
				continue;
		}
#endif
		
		// distance between a line (ax = y) and a point (x,y) in 2D space
		// D = |(-a * x) + y| / sqrt(a^2 + 1)

		//if ((planes[i]->position.x < 0.5) ? (fmodf(angularPos, M_PI * 2) < M_PI) : (fmodf(angularPos, M_PI * 2) > M_PI))
		//	continue;

		short quarter = planes[i]->position.x > 0.5 ? (planes[i]->position.y > 0.5 ? 0 : 1) : (planes[i]->position.y > 0.5 ? 3 : 2);
		if (scan_quarter != quarter)
			continue;

		float radarAngle = tanf(angularPos - (M_PI / 2));

		Vec2 normalizedPos = planes[i]->position - 0.5;
		normalizedPos.y = -normalizedPos.y;
		//printf("\nplane n-pos = {%f, %f}\n", normalizedPos.x, normalizedPos.y);

		float dist = fabsf((-normalizedPos.x * radarAngle + normalizedPos.y) / sqrt(1+ (radarAngle * radarAngle)));
		//printf("angle = %f, distance = %f\n", fmodf(angularPos, M_PI * 2), dist);

		float centerDist = planes[i]->position.dist(Vec2( 0.5, 0.5 ));
		//printf("center dist = %f\n", centerDist);

		if (dist < (0.08 * centerDist) && centerDist < 0.5 && centerDist > 0.05) {
			planes[i]->radarAge = PLANE_RADAR_MAX_AGE + 50;
			planes[i]->_spottedPos = ToRadarPos(planes[i]->position);
			WAV_HEADER header;
			header.SamplesPerSec *= sqrt(1.5 - centerDist); // Adjust the frequency (playback speed :3) to the distance from the center (cuz why not)
			header.bytesPerSec = header.SamplesPerSec * 2;
			PlaySound((LPCWSTR)&header, GetModuleHandle(NULL), SND_MEMORY | SND_ASYNC);
		}
	}

	for (int i = 0; i < airports.size(); i++)
	{
		if (airports[i]->_spottedPos.x >= 0) {

			if (selectedAirport) {
				if ((airports[i]->_spottedPos + Vec2(beginPos.x + RADAR_HALF_MARGIN, beginPos.y + RADAR_HALF_MARGIN)).dist(ImGui::GetMousePos()) < RADAR_SELECT_MARK_SIZE)
				{
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
						*selectedAirport = airports[i]->isSelected ? -1 : i;
						airports[i]->isSelected ^= true;
					}
					airports[i]->isHovered = true;
				}
			}

			// sqrt(3)/6 * RADAR_AIRPORT_SIZE
			const float triangleH = 0.288675134 * RADAR_AIRPORT_SIZE;

			ImVec2 TriangleBasePos = airports[i]->_spottedPos + RADAR_HALF_MARGIN + beginPos;
			dl->AddTriangleFilled(TriangleBasePos + Vec2(-RADAR_AIRPORT_SIZE/2, triangleH), TriangleBasePos + Vec2(RADAR_AIRPORT_SIZE/2, triangleH), TriangleBasePos + Vec2(0, -triangleH * 2), ImColor(0.1f, 0.95f, 0.1f, 0.99f));

			//dl->AddCircleFilled(airports[i]->_spottedPos + Vec2(beginPos.x + RADAR_HALF_MARGIN, beginPos.y + RADAR_HALF_MARGIN), 10, ImColor(0.1f, 0.95f, 0.1f, 0.5));

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(RADAR_TEXT_COLOR, 1.f));

			auto startPos = airports[i]->_spottedPos + RADAR_HALF_MARGIN + ImGui::WindowPosRelToAbs(ImGui::GetCurrentWindow(), { 0,0 });
			ImGui::RenderTextClipped({ startPos.x - 100, startPos.y - 50 }, { startPos.x + 100, startPos.y + 90 }, airports[i]->identifier, nullptr, nullptr, { 0.5, 0.5 });

			// Draw Rectangle Mark
			if (airports[i]->isSelected)
				DrawElementMark(startPos + Vec2(0, -triangleH/2), ImColor(RADAR_SELECT_MARK_COLOR, 1.0), dl, true);

			ImGui::PopStyleColor();

			continue;
		}

		short quarter = airports[i]->position.x > 0.5 ? (airports[i]->position.y > 0.5 ? 0 : 1) : (airports[i]->position.y > 0.5 ? 3 : 2);
		if (scan_quarter != quarter)
			continue;

		float radarAngle = tanf(angularPos - (M_PI / 2));

		Vec2 normalizedPos = airports[i]->position - 0.5;
		normalizedPos.y = -normalizedPos.y;
		//printf("\nplane n-pos = {%f, %f}\n", normalizedPos.x, normalizedPos.y);

		float dist = fabsf((-normalizedPos.x * radarAngle + normalizedPos.y) / sqrt(1 + (radarAngle * radarAngle)));
		//printf("angle = %f, distance = %f\n", fmodf(angularPos, M_PI * 2), dist);

		float centerDist = airports[i]->position.dist(Vec2(0.5, 0.5));
		//printf("center dist = %f\n", centerDist);

		if (dist < (0.05 * centerDist) && centerDist < 0.5 && centerDist > 0.05) {
			airports[i]->_spottedPos = ToRadarPos(airports[i]->position);

			WAV_HEADER header;
			header.SamplesPerSec *= 1.4; // Adjust the frequency (playback speed :3) to the distance from the center (cuz why not)
			header.bytesPerSec = header.SamplesPerSec * 2;
			PlaySound((LPCWSTR)&header, GetModuleHandle(NULL), SND_MEMORY | SND_ASYNC);
		}
	}
}
