#pragma once

#include <iostream>
#include <Windows.h>
#include <d3d11.h>
#include <aobscanner.h>
#include <MinHook.h>
#include <imgui_main.h>
#include <dllmain.h>

namespace DXGI
{
	const bool HookDXGI();

	const void UnhookDXGI();

	LONG WINAPI PresentDetour(IDXGISwapChain* swapChain, unsigned syncInterval, unsigned flags);
}