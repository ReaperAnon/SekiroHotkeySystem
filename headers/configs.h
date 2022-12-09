#pragma once

#include <Windows.h>
#include <string>
#include <imgui.h>
#include <gamekey.h>
#include <input_processing.h>

namespace Configs
{
	void ReadConfigFile();

	void SaveConfigFile();

	void ReloadSettings();
}