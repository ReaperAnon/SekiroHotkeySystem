#pragma once

#include <string>
#include <imgui.h>
#include <imgui_menu.h>
#include <data.h>
#include <combatarts.h>
#include <prosthetics.h>
#include <imgui_hotkey.h>
#include <input_processing.h>
#include <widget_menu.h>

namespace ConfigMenu
{
	extern float ConfigScale;

	bool IsConfigOpen();

	void QuitConfigMenu(void*);

	void OpenConfigMenu(void*);

	bool ShowConfigMenu();
}