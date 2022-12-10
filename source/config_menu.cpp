#include <config_menu.h>

float ConfigMenu::ConfigScale;

bool IsConfigOpenVar = false;

bool IsConfigStyleApplied = false;

const char* UsageModes[]
{
    "Never",
    "Always",
    "Mid-Air",
    "While Blocking",
    "While Blocking or Mid-Air"
};

const char* WidgetModes[]
{
    "List",
    "Highlight",
    "Minimalistic",
    "Highlight Without Background",
    "Minimalistic Without Background"
};

const char* WidgetPosition[]
{
    "Top Right",
    "Bottom Right",
    "Above Health Bar"
};

const char* WidgetColor[]
{
    "White",
    "Match UI"
};


bool ConfigMenu::IsConfigOpen()
{
    return IsConfigOpenVar;
}

void ConfigMenu::QuitConfigMenu(void*)
{
    IsConfigOpenVar = false;
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
}

void ConfigMenu::OpenConfigMenu(void*)
{
    if (Hooks::IsInMenu())
    {
        IsConfigOpenVar = !IsConfigOpenVar;
        ImGuiIO& io = ImGui::GetIO();
        if (IsConfigOpenVar)
        {
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        }
        else
        {
            io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
            io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
        }
    }
}

bool ConfigMenu::ShowConfigMenu()
{
    bool showMenu = Hooks::IsInMenu();

    IsConfigOpenVar &= showMenu;
    if (showMenu)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (!IsConfigStyleApplied)
        {
            io.FontGlobalScale = ConfigScale * ImGui::GetResolutionScale();
            IsConfigStyleApplied = true;
        }

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoResize;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_NoDecoration;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoScrollbar;
        windowFlags |= ImGuiWindowFlags_NoBackground;
        windowFlags |= ImGuiWindowFlags_NoBackground;

        if (ImGui::Begin("configmenu", nullptr, windowFlags))
        {
            if (ImGui::Button("Configuration"))
            {
                IsConfigOpenVar = !IsConfigOpenVar;
                if (IsConfigOpenVar)
                {
                    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
                    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
                }
                else
                {
                    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
                    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
                }
            }

            ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 2 - ImGui::GetWindowWidth() / 2, 0));
            if (IsConfigOpenVar)
            {
                float windowHeight = ImGui::GetWindowHeight();
                if (ImGui::Begin("Configuration", &IsConfigOpenVar, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoTitleBar))
                {
                    ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 2 - ImGui::GetWindowWidth() / 2, windowHeight));
                    if (ImGui::BeginTabBar("tabs", ImGuiTabBarFlags_FittingPolicyDefault_))
                    {
                        if (ImGui::BeginTabItem("Mod"))
                        {
                            // Basic Options
                            ImGui::Spacing();
                            ImGui::Spacing();

                            ImGui::Text("Use Combat Art on Equip:");
                            ImGui::ListBox("", &CAFunctions::CAUsageMode, UsageModes, 5);
                            ImGui::Spacing();

                            // Dedicated Combat Art Key
                            ImGui::Hotkey("Combat Art Key:", &Input::CombatArtKey);

                            ImGui::Spacing();
                            ImGui::Separator();
                            ImGui::Spacing();

                            if (ImGui::CollapsingHeader("Combat Arts"))
                            {
                                ImGui::Text("Combat Art Slots:");
                                if (ImGui::SliderInt("Combat Art Hotkeys", &CAFunctions::CombatArtSize, 1, 10, CAFunctions::CombatArtSize > 1 ? "%d slots" : "%d slot", ImGuiSliderFlags_AlwaysClamp))
                                {
                                    Input::CombatArtKeys.resize(CAFunctions::CombatArtSize);
                                    CAFunctions::PerformArraySetup(CAFunctions::CombatArtSize);
                                }

                                ImGui::Spacing();

                                int idx = 0;
                                for (auto& keyEntry : Input::CombatArtKeys)
                                {
                                    std::string keyName = "Combat Art ";
                                    if (ImGui::Hotkey((keyName + std::to_string(++idx) + ":").c_str(), &keyEntry))
                                    {
                                        WidgetMenu::UpdateCombatArtNames();
                                    }
                                }
                            }

                            ImGui::Spacing();
                            ImGui::Separator();
                            ImGui::Spacing();

                            if (ImGui::CollapsingHeader("Prosthetic Sets"))
                            {
                                ImGui::Text("Prosthetic Set Slots:");
                                if (ImGui::SliderInt("Prosthetic Set Hotkeys", &ProstheticFunctions::ProstheticSetSize, 1, 10, ProstheticFunctions::ProstheticSetSize > 1 ? "%d slots" : "%d slot", ImGuiSliderFlags_AlwaysClamp))
                                {
                                    Input::ProstheticSetKeys.resize(ProstheticFunctions::ProstheticSetSize);
                                }

                                ImGui::Spacing();

                                int idx = 0;
                                for (auto& keyEntry : Input::ProstheticSetKeys)
                                {
                                    std::string keyName = "Prosthetic Set ";
                                    ImGui::Hotkey((keyName + std::to_string(++idx) + ":").c_str(), &keyEntry);
                                }
                            }

                            ImGui::Spacing();
                            ImGui::Separator();
                            ImGui::Spacing();

                            if (ImGui::CollapsingHeader("Prosthetics"))
                            {
                                ImGui::Hotkey("Prosthetic 1:", &Input::ProstheticKeys[0]);
                                ImGui::Hotkey("Prosthetic 2:", &Input::ProstheticKeys[1]);
                                ImGui::Hotkey("Prosthetic 3:", &Input::ProstheticKeys[2]);
                            }

                            ImGui::EndTabItem();
                        }

                        if (ImGui::BeginTabItem("Widget"))
                        {
                            ImGui::Spacing();
                            ImGui::Spacing();
                            if (ImGui::Checkbox("Show Associated Hotkey", &WidgetMenu::WidgetSettings.showHotkeys))
                            {
                                WidgetMenu::UpdateCombatArtNames();
                            }

                            ImGui::Spacing();

                            ImGui::SliderFloat("Widget Scale", &WidgetMenu::WidgetSettings.widgetScale, 0.5f, 3.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
                            ImGui::Spacing();
                            ImGui::Separator();

                            if (ImGui::ListBox("Widget Mode", &WidgetMenu::WidgetSettings.widgetMode, WidgetModes, 5))
                            {
                                if (WidgetMenu::WidgetSettings.widgetMode == 0)
                                    WidgetMenu::WidgetSettings.widgetPosition = 0;
                            }
                            ImGui::Spacing();
                            ImGui::Separator();

                            if (ImGui::ListBox("Widget Position", &WidgetMenu::WidgetSettings.widgetPosition, WidgetPosition, 3))
                            {
                                if (WidgetMenu::WidgetSettings.widgetPosition > 0 && WidgetMenu::WidgetSettings.widgetMode == 0)
                                    WidgetMenu::WidgetSettings.widgetMode = 1;
                            }
                            ImGui::Spacing();
                            ImGui::Separator();

                            ImGui::ListBox("Widget Highlight Color", &WidgetMenu::WidgetSettings.widgetColor, WidgetColor, 2);
                            ImGui::EndTabItem();
                        }

                        if (ImGui::BeginTabItem("Config"))
                        {
                            ImGui::Spacing();
                            ImGui::Spacing();
                            if (ImGui::SliderFloat("Config Menu Scale", &ConfigScale, 0.5f, 3.f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
                            {
                                ImGuiIO& io = ImGui::GetIO();
                                io.FontGlobalScale = ConfigScale * ImGui::GetResolutionScale();
                            }

                            ImGui::EndTabItem();
                        }

                        ImGui::EndTabBar();
                    }

                    ImGui::End();
                }
            }

            ImGui::End();
        }

        return IsConfigOpenVar;
    }

    IsConfigStyleApplied = false;
    return IsConfigOpenVar;
}
