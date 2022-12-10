#include <widget_menu.h>

bool IsWidgetStyleApplied = false;

ImGui::WidgetSettings WidgetMenu::WidgetSettings;

std::vector<std::string> WidgetMenu::CombatArtNames;


unsigned int WidgetMenu::GetRealIDFromMenuID(uintptr_t menuID)
{
    uint64_t EquipInventoryDataPtr = Hooks::GetEquipInventoryData();
    if (!EquipInventoryDataPtr)
        return 0;

    for (unsigned realID = 5000; realID < 10000; realID += 100)
    {
        uintptr_t bMenuID = Hooks::GetMenuID(EquipInventoryDataPtr + 0x10, &realID);
        if (bMenuID == menuID)
            return realID;
    }

    return 0;
}

std::string WidgetMenu::GetNameFromRealID(unsigned realID)
{
    if (realID == 5000 || realID == 110000) //returns <?null?> so override with "Empty"
        return "EMPTY";

    uint64_t msgRepository = *reinterpret_cast<uint64_t*>(0x143D964C8);
    if (!msgRepository)
        return "EMPTY";

    uint64_t txtPtr = reinterpret_cast<uint64_t>(Hooks::GetItemName(msgRepository, realID));
    if (txtPtr == 0)
        return "EMPTY";

    std::wstring stringToConvert = std::wstring(reinterpret_cast<const wchar_t*>(txtPtr));
    if (stringToConvert.empty()) return std::string();

    int strSize = WideCharToMultiByte(CP_UTF8, 0, &stringToConvert[0], (int)stringToConvert.size(), NULL, 0, NULL, NULL);

    std::string convertedString(strSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, &stringToConvert[0], (int)stringToConvert.size(), &convertedString[0], strSize, NULL, NULL);
    return convertedString;
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
            CombatArtNames[i] = nameBase + GetNameFromRealID(realID);
        else
            CombatArtNames[i] = nameBase + "EMPTY";
    }
}

void WidgetMenu::ShowWidgetMenu()
{
    if (!Hooks::IsGameLoaded())
        return;

    if (WidgetSettings.widgetMode == 0 && CAFunctions::IsArrayEmpty())
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
        if (WidgetSettings.widgetMode > 2)
            windowFlags |= ImGuiWindowFlags_NoBackground;

        if (WidgetSettings.widgetPosition == 0)
        {
            ImVec2 widgetPos = ImGui::GetScaledScreenCoord(ImVec2(1490, 120));
            ImGui::SetNextWindowPos(widgetPos, ImGuiCond_Always);
        }
        else if (WidgetSettings.widgetPosition == 1)
        {
            ImVec2 widgetPos = ImGui::GetScaledScreenCoord(ImVec2(1515, WidgetSettings.widgetMode > 2 ? 1015 : 1030));
            ImGui::SetNextWindowPos(widgetPos, ImGuiCond_Always);
        }
        else
        {
            ImVec2 widgetPos = ImGui::GetScaledScreenCoord(ImVec2(250, WidgetSettings.widgetMode > 2 ? 912 : 912));
            ImGui::SetNextWindowPos(widgetPos, ImGuiCond_Always);
        }

        if (!ImGui::Begin("cadisplay", nullptr, windowFlags))
            return;

        unsigned currentMenuID = *reinterpret_cast<unsigned*>(Hooks::GetSkillBase() + 0x28);
        if (WidgetSettings.widgetMode == 0)
        {
            for (int i = 0; i < CombatArtNames.size(); i++)
            {
                ImVec4 textColor = CAFunctions::CombatArts[i] == currentMenuID ? WidgetSettings.widgetColor == 1 ? ImVec4(0.51f, 0.251f, 0.196f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_Text] : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_TextDisabled];
                ImGui::TextColored(ImColor(textColor), CombatArtNames[i].c_str());

                if (i != CombatArtNames.size() - 1)
                    ImGui::Separator();
            }
        }
        else if (WidgetSettings.widgetMode == 1 || WidgetSettings.widgetMode == 3)
        {
            unsigned realID = GetRealIDFromMenuID(currentMenuID);
            std::string artName = GetNameFromRealID(realID);

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
            if (ImGui::BeginTable("caWidgetTable", CombatArtNames.size(), WidgetSettings.widgetMode > 2 ? ImGuiTableFlags_NoBordersInBody : ImGuiTableFlags_Borders))
            {
                for (int i = 0; i < CombatArtNames.size(); i++)
                {
                    ImGui::TableNextColumn();
                    if (WidgetSettings.showHotkeys)
                    {
                        ImVec4 textColor = CAFunctions::CombatArts[i] == currentMenuID ? WidgetSettings.widgetColor == 1 ? ImVec4(0.51f, 0.251f, 0.196f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_Text] : WidgetSettings.widgetMode == 3 ? ImVec4(0.247f, 0.255f, 0.251f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_TextDisabled];
                        ImGui::TextColored(textColor, ImGui::GetKeyName(Input::CombatArtKeys[i]).c_str());
                    }
                    else
                    {
                        ImGui::Text("     ");
                        ImDrawList* drawList = ImGui::GetForegroundDrawList();
                        ImVec4 circleColor = CAFunctions::CombatArts[i] == currentMenuID ? WidgetSettings.widgetColor == 1 ? ImVec4(0.51f, 0.251f, 0.196f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_Text] : WidgetSettings.widgetMode == 3 ? ImVec4(0.247f, 0.255f, 0.251f, 1.f) : ImGui::GetCurrentContext()->Style.Colors[ImGuiCol_TextDisabled];
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
