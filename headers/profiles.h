#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <data.h>
#include <functions.h>
#include <combatarts.h>
#include <prosthetics.h>

namespace Profiles
{
	uint64_t DeleteSaveHook(uint64_t arg1, uint64_t arg2, unsigned saveSlot);

	unsigned GetProfileIndex(std::filesystem::path profilePath);

	void InitProfileManager();

	void UninitProfileManager();

	void ManageProfiles();

	void DeleteProfile(unsigned slotIdx);

	void SaveProfile();

	void LoadProfile();
}