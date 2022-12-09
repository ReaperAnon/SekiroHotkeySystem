#pragma once

#include <locale>
#include <Windows.h>
#include <vector>

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
}