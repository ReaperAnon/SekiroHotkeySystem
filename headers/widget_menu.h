#pragma once

#include <string>
#include <vector>
#include <imgui.h> 
#include <imgui_menu.h>
#include <data.h>
#include <functions.h>
#include <combatarts.h>
#include <imgui_hotkey.h>
#include <selection_menu.h>

namespace WidgetMenu
{
	extern ImGui::WidgetSettings WidgetSettings;

	extern std::vector<std::string> CombatArtNames;


	unsigned int GetRealIDFromMenuID(uintptr_t menuID);

	void UpdateCombatArtNames();

	void ShowWidgetMenu();
}