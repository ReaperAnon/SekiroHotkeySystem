#pragma once

#include <Windows.h>
#include <string>
#include <imgui.h>
#include <gamekey.h>
#include <input_processing.h>
#include <selection_menu.h>

namespace Configs
{
	void ReadConfigFile();

	void SaveConfigFile();

	bool GameKeyCompare(const Input::GameKey* gameKey1, const Input::GameKey* gameKey2);

	void ReloadSettings();
}