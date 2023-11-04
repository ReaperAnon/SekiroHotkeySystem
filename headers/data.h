#pragma once

#include <locale>
#include <Windows.h>
#include <vector>
#include <selection_menu.h>
#include <dllmain.h>

#define COMBAT_ART_MIN 5000
#define COMBAT_ART_MAX 10000

#define PROSTHETIC_MIN 70000
#define PROSTHETIC_MAX 100000

#define PROSTHETIC_EMPTY_SLOT 256

namespace Hooks
{
	bool IsInMenu();

	bool IsHUDVisible();

	int GetSaveSlotIdx();


	byte* ResolvePointerChain(byte* baseAddress, std::vector<int> offsets);

	uint64_t GetSkillBase();

	uint64_t GetCharacterBase();

	byte* GetInputHandler();

	byte* GetUIValuesBase();

	uint64_t GetPlayerGameData();

	uint64_t GetEquipInventoryData();

	bool IsGameLoaded();

	std::vector<unsigned> GetCombatArts();

	std::vector<unsigned> GetProstheticTools();

	void GetCombatArtEquipSet(int idx);

	void GetProstheticEquipSet();

	std::string GetNameFromRealID(unsigned realID);

#ifdef HOTKEYS_DEBUG
	void PrintIDs(void *);
#endif
}