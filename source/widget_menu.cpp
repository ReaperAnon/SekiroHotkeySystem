#include <widget_menu.h>

bool IsWidgetStyleApplied = false;

ImGui::WidgetSettings WidgetMenu::WidgetSettings;

std::vector<std::string> WidgetMenu::CombatArtNames;


unsigned int WidgetMenu::GetRealIDFromMenuID(uintptr_t menuID)
{
    uint64_t EquipInventoryDataPtr = Hooks::GetEquipInventoryData();

    if (!EquipInventoryDataPtr)
        return 0;

    for (unsigned realID = COMBAT_ART_MIN; realID < COMBAT_ART_MAX; realID += 100)
    {
        uintptr_t bMenuID = Hooks::GetMenuID(EquipInventoryDataPtr + 0x10, &realID);
        if (bMenuID == menuID)
            return realID;
    }

    return 0;
}

void WidgetMenu::UpdateCombatArtNames()
{
    CombatArtNames.resize(CAFunctions::CombatArts.size());
    for (int i = 0; i < CAFunctions::CombatArts.size(); i++)
    {
        std::string nameBase = "";
        if (WidgetSettings.showHotkeys)
        {
            nameBase += ImGui::GetKeyName(Input::CombatArtKeys[i]);
            nameBase += ". ";
        }

        unsigned realID = GetRealIDFromMenuID(CAFunctions::CombatArts[i]);
        if (realID != 0)
            CombatArtNames[i] = nameBase + Hooks::GetNameFromRealID(realID);
        else
            CombatArtNames[i] = nameBase + "EMPTY";
    }
}

void WidgetMenu::ShowWidgetMenu()
{
    if (WidgetSettings.widgetMode == 0)
        return;

    if (SelectionMenu::IsMenuOpen)
        return;

    if (!Hooks::IsGameLoaded())
        return;

    if (WidgetSettings.widgetMode == 1 && CAFunctions::IsArrayEmpty())
        return;

    if (Hooks::IsHUDVisible())
    {
        if (CombatArtNames.size() == 0)
            UpdateCombatArtNames();

        if (!IsWidgetStyleApplied)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.FontGlobalScale = WidgetSettings.widgetScale * ImGui::GetResolutionScale();
            IsWidgetStyleApplied = true;
        }

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoResize;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoScrollbar;
        windowFlags |= ImGuiWindowFlags_NoMove;
        if (WidgetSettings.widgetMode > 3)
            windowFlags |= ImGuiWindowFlags_NoBackground;

        if (WidgetSettings.widgetPosition == 0)
        {
            ImVec2 widgetPos = ImGui::GetScaledScreenCoord(ImVec2(1490.f, 120.f));
            ImGui::SetNextWindowPos(widgetPos, ImGuiCond_Always);
        }
        else if (WidgetSettings.widgetPosition == 1)
        {
            ImVec2 widgetPos = ImGui::GetScaledScreenCoord(ImVec2(1515.f, WidgetSettings.widgetMode > 3 ? 1015.f : 1030.f));
            ImGui::SetNextWindowPos(widgetPos, ImGuiCond_Always);
        }
        else
        {
            ImVec2 widgetPos = ImGui::GetScaledScreenCoord(ImVec2(250.f, WidgetSettings.widgetMode > 2 ? 912.f : 912.f));
            ImGui::SetNextWindowPos(widgetPos, ImGuiCond_Always);
        }

        if (!ImGui::Begin("cadisplay", nullptr, windowFlags))
            return;

        unsigned currentMenuID = *reinterpret_cast<unsigned*>(Hooks::GetSkillBase() + 0x28);
        if (WidgetSettings.widgetMode == 1)
        {
            for (int i = 0; i < CombatArtNames.size(); i++)
            {
                ImVec4 textColor = CAFunctions::CombatArts[i] == currentMenuID ? WidgetSettings.widgetColor == 1 ? ImVec4(0.51f, 0.251f, 0.196f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_Text] : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_TextDisabled];
                ImGui::TextColored(ImColor(textColor), CombatArtNames[i].c_str());

                if (i != CombatArtNames.size() - 1)
                    ImGui::Separator();
            }
        }
        else if (WidgetSettings.widgetMode == 2 || WidgetSettings.widgetMode == 4)
        {
            unsigned realID = GetRealIDFromMenuID(currentMenuID);
            std::string artName = Hooks::GetNameFromRealID(realID);

            if (WidgetSettings.showHotkeys)
            {
                for (int i = 0; i < CAFunctions::CombatArts.size(); i++)
                {
                    if (CAFunctions::CombatArts[i] == currentMenuID)
                    {
                        artName = CombatArtNames[i];
                        break;
                    }
                }
            }

            ImVec4 textColor = WidgetSettings.widgetColor == 1 ? ImVec4(0.51f, 0.251f, 0.196f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_Text];
            ImGui::TextColored(ImColor(textColor), artName.c_str());
        }
        else
        {
            if (ImGui::BeginTable("caWidgetTable", CombatArtNames.size(), WidgetSettings.widgetMode > 3 ? ImGuiTableFlags_NoBordersInBody : ImGuiTableFlags_Borders))
            {
                for (int i = 0; i < CombatArtNames.size(); i++)
                {
                    ImGui::TableNextColumn();
                    if (WidgetSettings.showHotkeys)
                    {
                        ImVec4 textColor = CAFunctions::CombatArts[i] == currentMenuID ? WidgetSettings.widgetColor == 1 ? ImVec4(0.51f, 0.251f, 0.196f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_Text] : WidgetSettings.widgetMode == 4 ? ImVec4(0.247f, 0.255f, 0.251f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_TextDisabled];
                        ImGui::TextColored(textColor, ImGui::GetKeyName(Input::CombatArtKeys[i]).c_str());
                    }
                    else
                    {
                        ImGui::Text("   ");
                        ImDrawList* drawList = ImGui::GetForegroundDrawList();
                        ImVec4 circleColor = CAFunctions::CombatArts[i] == currentMenuID ? WidgetSettings.widgetColor == 1 ? ImVec4(0.51f, 0.251f, 0.196f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_Text] : WidgetSettings.widgetMode == 4 ? ImVec4(0.247f, 0.255f, 0.251f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_TextDisabled];
                        ImVec2 circlePos = ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), i).GetCenter();
                        static float circleHeight;
                        if (i > 0)
                            circleHeight = circlePos.y;
                        else
                            circlePos.y = circleHeight;

                        drawList->AddCircleFilled(circlePos, 4.5f * WidgetSettings.widgetScale * ImGui::GetResolutionScale(), ImColor(circleColor), 100);
                    }
                }

                ImGui::EndTable();
            }
        }

        ImGui::End();
        return;
    }

    IsWidgetStyleApplied = false;
}
