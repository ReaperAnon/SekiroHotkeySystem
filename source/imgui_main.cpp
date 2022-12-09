#include <imgui_main.h>

WNDPROC Input::OLDWNDPROC;

bool ImGui::CheckRestrictInputs(uintptr_t arg1, float arg2)
{
    return ConfigMenu::IsConfigOpen() ? true : Hooks::IsMenuInputActiveOrig(arg1, arg2);
}

bool ImGui::CheckRestrictInputsNoArgs()
{
    return ConfigMenu::IsConfigOpen() ? true : Hooks::IsMenuInputActiveNoArgsOrig();
}

bool ImGui::CheckRestrictInputsGamepad(uint64_t padDevicePtr)
{
    if (ConfigMenu::IsConfigOpen())
    {
        uint64_t a = *reinterpret_cast<uint64_t*>(0x143F42850);
        if (!a)
            return false;

        uint64_t b = *reinterpret_cast<uint64_t*>(a + 0x18);
        if (!b)
            return false;

        uint64_t c = *reinterpret_cast<uint64_t*>(b + 0x10); //gamepad0
        if (!c)
            return false;

        *reinterpret_cast<float*>(c + 0x24C) = 0; //analog sticks, so that they don't freeze at a non-zero number, turning the camera around endlessly.
        *reinterpret_cast<float*>(c + 0x250) = 0;
        *reinterpret_cast<float*>(c + 0x258) = 0;
        *reinterpret_cast<float*>(c + 0x25C) = 0;
    }

    return Hooks::ParseGamepadInputOrig(padDevicePtr);
}

void ImGui::InitImGuiDX11(ID3D11Device* D3D11Device, ID3D11DeviceContext* D3D11DeviceContext)
{
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = ".\\configs\\imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF(".\\resource\\Athelas-Regular.ttf", 17.f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 2;
    style.ChildRounding = 1;
    style.FrameRounding = 3;
    style.PopupRounding = 3;
    style.FramePadding = ImVec2(12, 3);
    style.ItemInnerSpacing = ImVec2(9, 3);
    style.WindowMenuButtonPosition = ImGuiDir_None;

    ImGui_ImplDX11_Init(D3D11Device, D3D11DeviceContext);

}

void ImGui::InitImGuiWin32()
{
    // Initialize input processing for ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(Input::PROCHWND);
    Input::OLDWNDPROC = (WNDPROC)SetWindowLongPtrW(Input::PROCHWND, GWLP_WNDPROC, (LONG_PTR)Input::WndProc);
}

void ImGui::UninitImGui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    DestroyContext();
    SetWindowLongPtr(Input::PROCHWND, GWLP_WNDPROC, (LONG_PTR)Input::OLDWNDPROC);
}

void ImGui::ImGuiPresent(ID3D11DeviceContext* D3D11DeviceContext, ID3D11RenderTargetView* D3D11RenderTargetView)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // ImGui::ShowDemoWindow();
    WidgetMenu::ShowWidgetMenu();

    static bool needRefreshConfig = false;
    if (!ConfigMenu::ShowConfigMenu())
    {
        if (needRefreshConfig)
        {
            needRefreshConfig = false;
            Configs::ReloadSettings();
            Configs::SaveConfigFile();
            WidgetMenu::UpdateCombatArtNames();
        }
    }
    else needRefreshConfig = true;

    ImGui::EndFrame();
    ImGui::Render();

    D3D11DeviceContext->OMSetRenderTargets(1, &D3D11RenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}