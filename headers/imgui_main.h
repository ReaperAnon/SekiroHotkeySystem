#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <config_menu.h>
#include <widget_menu.h>
#include <d3d11.h>
#include <input_processing.h>
#include <wndproc.h>
#include <functions.h>
#include <configs.h>

namespace ImGui
{
	bool CheckRestrictInputs(uintptr_t arg1, float arg2);

	bool CheckRestrictInputsNoArgs();

	bool CheckRestrictInputsGamepad(uint64_t padDevicePtr);

	void InitImGuiDX11(ID3D11Device* D3D11Device, ID3D11DeviceContext* D3D11DeviceContext);

	void InitImGuiWin32();

	void UninitImGui();
	
	void ImGuiPresent(ID3D11DeviceContext* D3D11DeviceContext, ID3D11RenderTargetView* D3D11RenderTargetView);
}