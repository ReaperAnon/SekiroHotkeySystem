#pragma once

#include <string>
#include <imgui.h>
#include <data.h>
#include <combatarts.h>
#include <prosthetics.h>

namespace SelectionMenu
{
	struct EquipEntry
	{
		std::string equipName = "";
		uint64_t realID = 0;
		uint64_t menuID = 0;
	};

	extern bool IsMenuOpen;

	extern bool ProstheticMode;

	extern std::vector<EquipEntry> EquipEntries;

	void SetShowArtMenu(void *);

	void SetShowProstheticMenu(void *);

	void CloseMenu(void *);

	bool ShowSelectionMenu();
}