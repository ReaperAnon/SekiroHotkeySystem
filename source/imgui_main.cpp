#include <imgui_main.h>

WNDPROC Input::OLDWNDPROC = nullptr;

float ImGui::TimeMult = 0.f;


bool ImGui::RestrictUpdateUIHook(uintptr_t arg1, float arg2) // stops the menu from being updated, causes floating markers
{
    return ConfigMenu::IsConfigOpen() ? true : Hooks::RestrictUpdateUIOrig(arg1, arg2);
}

bool skipInput = false;
bool ImGui::IsMenuModeActiveHook() // makes mouse show up and blocks inputs
{
    if (SelectionMenu::IsMenuOpen)
    {
        skipInput = true;
        return true;
    }

    if (skipInput)
    {
        skipInput = false;
        return true;
    }

    return Hooks::IsMenuModeActiveOrig();
}

bool ImGui::CheckRestrictInputsGamepad(uint64_t padDevicePtr)
{
    if (ConfigMenu::IsConfigOpen() || SelectionMenu::IsMenuOpen)
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

double ImGui::ProcessAnalogInputHook(uint64_t PadDevicePtr, int index)
{
    if (SelectionMenu::IsMenuOpen)
        return 0;

    return Hooks::ProcessAnalogInputOrig(PadDevicePtr, index);
}

uint64_t ImGui::GetGameSpeedHook(uint64_t arg1, uint64_t timePerFrame)
{
    if(SelectionMenu::IsMenuOpen)
        *reinterpret_cast<float*>(timePerFrame + 0x8) = 0.016666667f * TimeMult; // normal time per frame is 1/60.

    return Hooks::GetGameSpeedOrig(arg1, timePerFrame);
}

void ImGui::InitImGuiDX11(ID3D11Device* D3D11Device, ID3D11DeviceContext* D3D11DeviceContext)
{
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = ".\\configs\\imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.93f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.23f, 0.23f, 0.23f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.23f, 0.23f, 0.23f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.33f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);

    ImFontConfig config;
    config.MergeMode = true;
    /*
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSerifEN-Bold.ttf", 28, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSerifRU-Bold.ttf", 28, &config, io.Fonts->GetGlyphRangesCyrillic());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSerifJP-Bold.otf", 28, &config, io.Fonts->GetGlyphRangesJapanese());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSerifTC-Bold.otf", 28, &config, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSerifSC-Bold.otf", 28, &config, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSerifKR-Bold.otf", 28, &config, io.Fonts->GetGlyphRangesKorean());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSerifThai-Bold.ttf", 28, &config, io.Fonts->GetGlyphRangesThai());
    */

    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSansEN.ttf", 28, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSansRU-Bold.ttf", 28, &config, io.Fonts->GetGlyphRangesCyrillic());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSansJP-Bold.otf", 28, &config, io.Fonts->GetGlyphRangesJapanese());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSansTC-Bold.otf", 28, &config, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSansSC-Bold.otf", 28, &config, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSansKR-Bold.otf", 28, &config, io.Fonts->GetGlyphRangesKorean());
    io.Fonts->AddFontFromFileTTF(".\\resource\\NotoSansThai-Bold.ttf", 28, &config, io.Fonts->GetGlyphRangesThai());

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 2;
    style.ChildRounding = 1;
    style.FrameRounding = 3;
    style.PopupRounding = 3;
    style.FramePadding = ImVec2(12, 3);
    style.ItemInnerSpacing = ImVec2(9, 3);
    style.WindowMenuButtonPosition = ImGuiDir_None;

    ImGui_ImplDX11_Init(D3D11Device, D3D11DeviceContext)
#ifdef HOTKEYS_DEBUG
        ? std::cout << "ImGui Display Hooked\n" : std::cout << "ImGui Display Hook Failed\n"
#endif
        ;
}

void ImGui::InitImGuiWin32()
{
    // Initialize input processing for ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(Input::PROCHWND)
#ifdef HOTKEYS_DEBUG
        ? std::cout << "ImGui Input Hooked\n" : std::cout << "ImGui Input Hook Failed\n"
#endif
        ;
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
    SelectionMenu::ShowSelectionMenu();

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