#pragma once

#include <d3d11.h>
#include <vector>

namespace GUI
{
	HWND Setup(int (*OnGuiFunc)());
	int DrawGui() noexcept;
	void Destroy() noexcept;
	void LoadFonts(float fontSizeMultiplier = 1.f);

	inline bool (*onExitFunc)();

	static int windowX = 1280, windowY = 756;

#ifdef _DEBUG
#define SYNC_FRAMES 2
#else
#define SYNC_FRAMES 1
#endif
}

#define TOOLTIP(...)					\
   	if (ImGui::IsItemHovered())			\
		ImGui::SetTooltip(__VA_ARGS__); \

#define TOOLTIPFONT(...)														\
   	if (ImGui::IsItemHovered())	{												\
		ImGui::PushFont(io.Fonts->Fonts[fontIndex[currentUserData.style.selectedFont]]);	\
		ImGui::SetTooltip(__VA_ARGS__);											\
		ImGui::PopFont(); } 

#define REVERT(mainVar, bakVar)		\
	if(ImGui::IsItemClicked(1))		\
		*mainVar = bakVar;

#define IS_ONLY_ENTER_PRESSED ((ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) && ImGui::GetIO().InputQueueCharacters.size() <= 1)
#define IS_ONLY_ESCAPE_PRESSED (ImGui::IsKeyPressed(ImGuiKey_Escape) && ImGui::GetIO().InputQueueCharacters.size() <= 1)