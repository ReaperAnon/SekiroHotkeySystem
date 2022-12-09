#pragma once

#define FUNCWRAPPER HRESULT __declspec(dllexport) WINAPI

#include <Windows.h>
#include <imgui_main.h>
#include <input_processing.h>
#include <dxgi_inject.h>
#include <configs.h>
#include <profiles.h>
#include <functions.h>

// A pointer type for the original dinput8 dll function.
typedef HRESULT(__stdcall* tDirectInput8Create)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

void HookThread(void* args);

// Loads the specified chain dll or if one is not defined, the original dinput8.dll.
void LoadDll();

// Event function which runs when the current process makes a call to the dll.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason_for_call, LPVOID lpReserved);