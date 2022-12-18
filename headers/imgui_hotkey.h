#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <string>
#include <imgui.h>
#include <imgui_internal.h>
#include <gamekey.h>

namespace ImGui
{
	extern bool IsHotkeyBeingSet;

	const std::string GetKeyName(Input::ImGuiKeySet keyID);

	bool KeyPressBehavior(const bool isHovered, const bool isSelected, bool* focusReleasedWithNav, bool* isMouseActivation);

	bool Hotkey(const char* label, Input::ImGuiKeySet* keysToSet, const ImVec2& size_arg = ImVec2(0, 0));

	bool Hotkey(const char* label, ImGuiKey* keyToSet, const ImVec2& size_arg = ImVec2(0, 0));
}
