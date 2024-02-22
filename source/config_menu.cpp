#include <config_menu.h>

float ConfigMenu::ConfigScale;

bool IsConfigOpenVar = false;

bool IsConfigStyleApplied = false;

const char* UsageModes[]
{
    "Never",
    "Always",
    "Custom"
};

const char* WidgetModes[]
{
    "Off",
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

const char* Fonts[]
{
    "Noto Sans",
    "Noto Serif"
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
        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoResize;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_NoDecoration;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoScrollbar;
        windowFlags |= ImGuiWindowFlags_NoBackground;
        windowFlags |= ImGuiWindowFlags_NoBackground;
        
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f, 0), NULL, ImVec2(0.5f, 0.f));
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

            if (IsConfigOpenVar)
            {
                ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f, ImGui::GetWindowHeight()), NULL, ImVec2(0.5f, 0.f));
                if (ImGui::Begin("Configuration", &IsConfigOpenVar, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoTitleBar))
                {
                    if (ImGui::BeginTabBar("tabs", ImGuiTabBarFlags_FittingPolicyDefault_))
                    {
                        if (ImGui::BeginTabItem("Combat Arts"))
                        {
                            ImGui::Spacing();
                            ImGui::Spacing();

                            ImGui::Text("Use Combat Art on Equip:");
                            ImGui::ListBox("Combat Art Mode", &CAFunctions::CAUsageMode, UsageModes, 3);
                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("Controls when and how combat arts should be used automatically (if at all). The custom option enables finer conditional controls.");
                                ImGui::EndTooltip();
                            }
                            ImGui::Spacing();

                            if (CAFunctions::CAUsageMode == 2)
                            {
                                ImGui::Checkbox("Use While In Air", &CAFunctions::UseWhileInAir);
                                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::SetTooltip("If enabled, combat arts will be used automatically if equipped while in the air.");
                                    ImGui::EndTooltip();
                                }

                                ImGui::Spacing();
                                ImGui::Checkbox("Use While Blocking", &CAFunctions::UseWhileBlocking);
                                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::SetTooltip("If enabled, combat arts will be used automatically if equipped while blocking.");
                                    ImGui::EndTooltip();
                                }

                                ImGui::Spacing();
                                ImGui::Checkbox("Use When Repeated", &CAFunctions::UseOnRepeat);
                                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::SetTooltip("If enabled, combat arts will be used automatically if equipped again while they're already selected.");
                                    ImGui::EndTooltip();
                                }

                                ImGui::Spacing();
                            }

                            ImGui::Separator();
                            ImGui::Spacing();

                            ImGui::Hotkey("Combat Art Key:", &Input::CombatArtKey);
                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("Hotkey for using combat arts with a single button press.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Spacing();
                            ImGui::Separator();
                            ImGui::Spacing();

                            ImGui::Text("Combat Art Slots:");
                            if (ImGui::SliderInt("Combat Art Hotkeys", &CAFunctions::CombatArtSize, 1, 19, CAFunctions::CombatArtSize > 1 ? "%d slots" : "%d slot", ImGuiSliderFlags_AlwaysClamp))
                            {
                                Input::CombatArtKeys.resize(CAFunctions::CombatArtSize);
                                CAFunctions::PerformArraySetup(CAFunctions::CombatArtSize);
                            }

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("The number of assignable combat art hotkeys.");
                                ImGui::EndTooltip();
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

                                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::SetTooltip("Combat art slot hotkey. Combat arts can be saved into the slot by pressing the hotkey in-game if the slot is empty, pressing the key while in the pause menu\n or by pressing it along with the Equip Modifier key to bring up a selection menu.");
                                    ImGui::EndTooltip();
                                }
                            }

                            ImGui::EndTabItem();
                        }

                        if (ImGui::BeginTabItem("Prosthetics"))
                        {
                            ImGui::Spacing();
                            ImGui::Spacing();

                            ImGui::Text("Use Prosthetic on Equip:");
                            ImGui::ListBox("Prosthetic Mode", &ProstheticFunctions::ProstheticUsageMode, UsageModes, 3);
                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("Controls when and how prosthetics should be used automatically (if at all). The custom option enables finer conditional controls.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Spacing();

                            if (ProstheticFunctions::ProstheticUsageMode == 2)
                            {
                                ImGui::Checkbox("Use While In Air", &ProstheticFunctions::UseWhileInAir);
                                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::SetTooltip("If enabled, prosthetics will be used automatically if equipped while in the air.");
                                    ImGui::EndTooltip();
                                }

                                ImGui::Spacing();
                                ImGui::Checkbox("Use While Blocking", &ProstheticFunctions::UseWhileBlocking);
                                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::SetTooltip("If enabled, prosthetics will be used automatically if equipped while blocking.");
                                    ImGui::EndTooltip();
                                }

                                ImGui::Spacing();
                                ImGui::Checkbox("Use When Repeated", &ProstheticFunctions::UseOnRepeat);
                                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::SetTooltip("If enabled, prosthetics will be used automatically if equipped again while they're already selected.");
                                    ImGui::EndTooltip();
                                }
                            }

                            ImGui::Separator();
                            ImGui::Spacing();

                            ImGui::Hotkey("Prosthetic 1:", &Input::ProstheticKeys[0]);
                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("Hotkey for selecting the prosthetic in the first slot.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Hotkey("Prosthetic 2:", &Input::ProstheticKeys[1]);
                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("Hotkey for selecting the prosthetic in the second slot.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Hotkey("Prosthetic 3:", &Input::ProstheticKeys[2]);
                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("Hotkey for selecting the prosthetic in the third slot.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Spacing();
                            ImGui::Separator();
                            ImGui::Spacing();

                            ImGui::Text("Prosthetic Set Slots:");
                            if (ImGui::SliderInt("Prosthetic Set Hotkeys", &ProstheticFunctions::ProstheticSetSize, 1, 20, ProstheticFunctions::ProstheticSetSize > 1 ? "%d slots" : "%d slot", ImGuiSliderFlags_AlwaysClamp))
                            {
                                Input::ProstheticSetKeys.resize(ProstheticFunctions::ProstheticSetSize);
                            }

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("The number of assignable prosthetic set hotkeys.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Spacing();

                            int idx = 0;
                            for (auto& keyEntry : Input::ProstheticSetKeys)
                            {
                                std::string keyName = "Prosthetic Set ";
                                ImGui::Hotkey((keyName + std::to_string(++idx) + ":").c_str(), &keyEntry);

                                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::SetTooltip("Prosthetic set slot hotkey. Prosthetic sets can be saved into the slot by pressing the hotkey in-game if the slot is empty, pressing the key while in the pause menu\n or by pressing it along with the Equip Modifier key to bring up a selection menu.");
                                    ImGui::EndTooltip();
                                }
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

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("If enabled, the hotkeys for the combat art slots will be displayed alongside the names of the stored combat arts.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Spacing();

                            ImGui::SliderFloat("Widget Scale", &WidgetMenu::WidgetSettings.widgetScale, 0.5f, 3.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("The size of the combat art widget.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Spacing();
                            ImGui::Separator();
                            ImGui::Spacing();

                            ImGui::Text("Widget Display Mode");

                            ImGui::Spacing();

                            if (ImGui::ListBox("Style", &WidgetMenu::WidgetSettings.widgetMode, WidgetModes, 6))
                            {
                                if (WidgetMenu::WidgetSettings.widgetMode == 1)
                                    WidgetMenu::WidgetSettings.widgetPosition = 0;
                            }

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("The style of the combat art widget. The list can only be in the top right position.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Spacing();
                            ImGui::Spacing();

                            if (ImGui::ListBox("Position", &WidgetMenu::WidgetSettings.widgetPosition, WidgetPosition, 3))
                            {
                                if (WidgetMenu::WidgetSettings.widgetPosition > 0 && WidgetMenu::WidgetSettings.widgetMode == 1)
                                    WidgetMenu::WidgetSettings.widgetMode = 2;
                            }

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("The position of the combat art widget. The list can only be in the top right position while the other styles can take any of the given positions.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Spacing();
                            ImGui::Spacing();

                            ImGui::ListBox("Highlight Color", &WidgetMenu::WidgetSettings.widgetColor, WidgetColor, 2);

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("The color of the text highlight in the combat art widget. Setting it to Match UI will make the selection match the color of the health bar.");
                                ImGui::EndTooltip();
                            }

                            ImGui::EndTabItem();
                        }

                        if (ImGui::BeginTabItem("Equip Select"))
                        {
                            ImGui::Spacing();
                            ImGui::Spacing();

                            ImGui::Hotkey("Equip Modifier Key", &Input::EquipmentModifierKey);

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("If holding the equip modifier key while pressing a combat art or prosthetic set hotkey, a menu will open up to allow assigning to the slots directly.");
                                ImGui::EndTooltip();
                            }

                            ImGui::Spacing();
                            ImGui::Spacing();

                            ImGui::SliderFloat("Time Multiplier", &ImGui::TimeMult, 0.1f, 1.f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("The multiplier applied to the game's speed while the equipment select menu is open.");
                                ImGui::EndTooltip();
                            }
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

                            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                            {
                                ImGui::BeginTooltip();
                                ImGui::SetTooltip("The size of the configuration menu.");
                                ImGui::EndTooltip();
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
