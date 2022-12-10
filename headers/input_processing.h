#pragma once

#include <vector>
#include <gamekey.h>
#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_main.h>
#include <wndproc.h>
#include <thread>

namespace Input
{
	extern std::vector<ImGuiKeySet> CombatArtKeys;

	extern std::vector<ImGuiKeySet> ProstheticSetKeys;

	extern ImGuiKeySet ProstheticKeys[3];

	extern ImGuiKeySet CombatArtKey;

	extern std::vector<Input::GameKey*> MenuKeys;

	extern std::vector<Input::GameKey*> GameKeys;


	void InitInputProcessing();

	void UninitInputProcessing();

	void InputProcessThread(void* args);

	void ProcessInputArray(std::vector<Input::GameKey*>* inputArray);

	void ProcessKeyEvents();

	BOOL CALLBACK WorkerProc(HWND hwnd, LPARAM lParam);

	LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void PressKey(void* keysToPress);

	void ReleaseKey(void* keysToRelease);
}