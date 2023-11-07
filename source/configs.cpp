#include <configs.h>

void Configs::ReadConfigFile()
{
    WCHAR inputBuffer[50];
    WCHAR configName[] = L".\\configs\\sekirohotkeys.ini";

    Input::CombatArtKeys.clear();
    Input::ProstheticSetKeys.clear();

    short idx = 0;
    int keyValue1 = 0, keyValue2 = 0;
    do
    {
        Input::ImGuiKeySet keySet = { ImGuiKey_None, ImGuiKey_None };
        std::wstring keyName = L"Key";
        keyName += std::to_wstring(idx++);

        keyValue1 = GetPrivateProfileInt(L"Combat Arts", (keyName + std::to_wstring(1)).c_str(), -1, configName);
        keySet.key1 = keyValue1 > -1 ? (ImGuiKey)keyValue1 : ImGuiKey_None;

        keyValue2 = GetPrivateProfileInt(L"Combat Arts", (keyName + std::to_wstring(2)).c_str(), -1, configName);
        keySet.key2 = keyValue2 > -1 ? (ImGuiKey)keyValue2 : ImGuiKey_None;

        if((keyValue1 != -1 && keyValue2 != -1) || idx == 1)
            Input::CombatArtKeys.push_back(keySet);

    } while (keyValue1 != -1 && keyValue2 != -1);

    CAFunctions::CombatArtSize = Input::CombatArtKeys.size();

    idx = 0;
    do
    {
        Input::ImGuiKeySet keySet = { ImGuiKey_None, ImGuiKey_None };
        std::wstring keyName = L"Key";
        keyName += std::to_wstring(idx++);

        keyValue1 = GetPrivateProfileInt(L"Prosthetic Sets", (keyName + std::to_wstring(1)).c_str(), -1, configName);
        keySet.key1 = keyValue1 > -1 ? (ImGuiKey)keyValue1 : ImGuiKey_None;

        keyValue2 = GetPrivateProfileInt(L"Prosthetic Sets", (keyName + std::to_wstring(2)).c_str(), -1, configName);
        keySet.key2 = keyValue2 > -1 ? (ImGuiKey)keyValue2 : ImGuiKey_None;

        if ((keyValue1 != -1 && keyValue2 != -1) || idx == 1)
            Input::ProstheticSetKeys.push_back(keySet);

    } while (keyValue1 != -1 && keyValue2 != -1);

    ProstheticFunctions::ProstheticSetSize = Input::ProstheticSetKeys.size();

    CAFunctions::UseWhileBlocking = GetPrivateProfileInt(L"Combat Arts", L"UseWhenBlocking", 0, configName);
    CAFunctions::UseWhileInAir = GetPrivateProfileInt(L"Combat Arts", L"UseWhenAir", 0, configName);
    CAFunctions::UseOnRepeat = GetPrivateProfileInt(L"Combat Arts", L"UseWhenRepeating", 0, configName);

    ProstheticFunctions::UseWhileBlocking = GetPrivateProfileInt(L"Prosthetics", L"UseWhenBlocking", 0, configName);
    ProstheticFunctions::UseWhileInAir = GetPrivateProfileInt(L"Prosthetics", L"UseWhenAir", 0, configName);
    ProstheticFunctions::UseOnRepeat = GetPrivateProfileInt(L"Prosthetics", L"UseWhenRepeating", 0, configName);

    GetPrivateProfileString(L"Menu", L"configScale", L"0.6", inputBuffer, 50, configName);
    ConfigMenu::ConfigScale = std::stof(inputBuffer);
    GetPrivateProfileString(L"Menu", L"widgetScale", L"0.8", inputBuffer, 50, configName);
    WidgetMenu::WidgetSettings.widgetScale = std::stof(inputBuffer);
    WidgetMenu::WidgetSettings.showHotkeys = GetPrivateProfileInt(L"Menu", L"widgetDisplayKey", 1, configName);
    WidgetMenu::WidgetSettings.widgetMode = GetPrivateProfileInt(L"Menu", L"widgetMode", 0, configName);
    WidgetMenu::WidgetSettings.widgetPosition = GetPrivateProfileInt(L"Menu", L"widgetPosition", 0, configName);
    WidgetMenu::WidgetSettings.widgetColor = GetPrivateProfileInt(L"Menu", L"widgetColor", 0, configName);

    CAFunctions::CAUsageMode = GetPrivateProfileInt(L"General", L"UsageMode", 0, configName);
    ProstheticFunctions::ProstheticUsageMode = GetPrivateProfileInt(L"General", L"ProstheticUsageMode", 0, configName);

    Input::EquipmentModifierKey = (ImGuiKey)GetPrivateProfileInt(L"General", L"EquipmentModifierKey", 0, configName);

    GetPrivateProfileString(L"General", L"TimeMult", L"0.25", inputBuffer, 50, configName);
    ImGui::TimeMult = std::stof(inputBuffer);

    Input::ImGuiKeySet keySet = { ImGuiKey_None, ImGuiKey_None };
    keySet.key1 = (ImGuiKey)GetPrivateProfileInt(L"General", L"CombatArtKey1", 0, configName);
    keySet.key2 = (ImGuiKey)GetPrivateProfileInt(L"General", L"CombatArtKey2", 0, configName);
    Input::CombatArtKey = keySet;

    keySet.key1 = (ImGuiKey)GetPrivateProfileInt(L"Prosthetics", L"Key01", 0, configName);
    keySet.key2 = (ImGuiKey)GetPrivateProfileInt(L"Prosthetics", L"Key02", 0, configName);
    Input::ProstheticKeys[0] = keySet;

    keySet.key1 = (ImGuiKey)GetPrivateProfileInt(L"Prosthetics", L"Key11", 0, configName);
    keySet.key2 = (ImGuiKey)GetPrivateProfileInt(L"Prosthetics", L"Key12", 0, configName);
    Input::ProstheticKeys[1] = keySet;

    keySet.key1 = (ImGuiKey)GetPrivateProfileInt(L"Prosthetics", L"Key21", 0, configName);
    keySet.key2 = (ImGuiKey)GetPrivateProfileInt(L"Prosthetics", L"Key22", 0, configName);
    Input::ProstheticKeys[2] = keySet;

    Input::GameKey* escapeKey = new Input::GameKey({ ImGuiKey_Escape, ImGuiKey_None }, ConfigMenu::QuitConfigMenu);
    Input::MenuKeys.push_back(escapeKey);

    Input::GameKey* openKey = new Input::GameKey({ ImGuiKey_Tab, ImGuiKey_None }, ConfigMenu::OpenConfigMenu);
    Input::MenuKeys.push_back(openKey);

    Input::GameKey* openKeyGamepad = new Input::GameKey({ ImGuiKey_GamepadR3, ImGuiKey_None }, ConfigMenu::OpenConfigMenu);
    Input::MenuKeys.push_back(openKeyGamepad);

    ReloadSettings();
}

void Configs::SaveConfigFile()
{
    std::string configText;

    // Menus
    configText = "[Menu]\nconfigScale=";
    configText += std::to_string(ConfigMenu::ConfigScale);
    configText += "\nwidgetDisplayKey=";
    configText += WidgetMenu::WidgetSettings.showHotkeys ? std::to_string(1) : std::to_string(0);
    configText += "\nwidgetScale=";
    configText += std::to_string(WidgetMenu::WidgetSettings.widgetScale);
    configText += "\nwidgetMode=";
    configText += std::to_string(WidgetMenu::WidgetSettings.widgetMode);
    configText += "\nwidgetPosition=";
    configText += std::to_string(WidgetMenu::WidgetSettings.widgetPosition);
    configText += "\nwidgetColor=";
    configText += std::to_string(WidgetMenu::WidgetSettings.widgetColor);

    // General
    configText += "\n\n[General]\nUsageMode=";
    configText += std::to_string(CAFunctions::CAUsageMode);
    configText += "\nProstheticUsageMode=";
    configText += std::to_string(ProstheticFunctions::ProstheticUsageMode);
    configText += "\nCombatArtKey1=";
    configText += std::to_string(Input::CombatArtKey.key1);
    configText += "\nCombatArtKey2=";
    configText += std::to_string(Input::CombatArtKey.key2);
    configText += "\nEquipmentModifierKey=";
    configText += std::to_string(Input::EquipmentModifierKey);
    configText += "\nTimeMult=";
    configText += std::to_string(ImGui::TimeMult);

    // Combat Arts
    configText += "\n\n[Combat Arts]";
    configText += "\nUseWhenBlocking=";
    configText += CAFunctions::UseWhileBlocking ? std::to_string(1) : std::to_string(0);
    configText += "\nUseWhenAir=";
    configText += CAFunctions::UseWhileInAir ? std::to_string(1) : std::to_string(0);
    configText += "\nUseWhenRepeating=";
    configText += CAFunctions::UseOnRepeat ? std::to_string(1) : std::to_string(0);
    for (int i = 0; i < Input::CombatArtKeys.size(); i++)
    {
        configText += "\nKey" + std::to_string(i) + "1=" + std::to_string(Input::CombatArtKeys[i].key1);
        configText += "\nKey" + std::to_string(i) + "2=" + std::to_string(Input::CombatArtKeys[i].key2);
    }

    // Prosthetic Sets
    configText += "\n\n[Prosthetic Sets]";
    for (int i = 0; i < Input::ProstheticSetKeys.size(); i++)
    {
        configText += "\nKey" + std::to_string(i) + "1=" + std::to_string(Input::ProstheticSetKeys[i].key1);
        configText += "\nKey" + std::to_string(i) + "2=" + std::to_string(Input::ProstheticSetKeys[i].key2);
    }

    // Prosthetics
    configText += "\n\n[Prosthetics]";
    configText += "\nUseWhenBlocking=";
    configText += ProstheticFunctions::UseWhileBlocking ? std::to_string(1) : std::to_string(0);
    configText += "\nUseWhenAir=";
    configText += ProstheticFunctions::UseWhileInAir ? std::to_string(1) : std::to_string(0);
    configText += "\nUseWhenRepeating=";
    configText += ProstheticFunctions::UseOnRepeat ? std::to_string(1) : std::to_string(0);
    configText += "\nKey01=" + std::to_string(Input::ProstheticKeys[0].key1);
    configText += "\nKey02=" + std::to_string(Input::ProstheticKeys[0].key2);
    configText += "\nKey11=" + std::to_string(Input::ProstheticKeys[1].key1);
    configText += "\nKey12=" + std::to_string(Input::ProstheticKeys[1].key2);
    configText += "\nKey21=" + std::to_string(Input::ProstheticKeys[2].key1);
    configText += "\nKey22=" + std::to_string(Input::ProstheticKeys[2].key2);

    std::filesystem::path dir = std::filesystem::current_path() / "configs" / "sekirohotkeys.ini";
    std::ofstream configFile(dir.string());

    configFile << configText;
    configFile.close();
}

bool Configs::GameKeyCompare(const Input::GameKey* gameKey1, const Input::GameKey* gameKey2)
{
    int keyOneParams = (gameKey1->KeyParams.key1 != ImGuiKey_None) + (gameKey1->KeyParams.key2 != ImGuiKey_None) + (gameKey1->ModKeyParam != ImGuiKey_None);
    int keyTwoParams = (gameKey2->KeyParams.key1 != ImGuiKey_None) + (gameKey2->KeyParams.key2 != ImGuiKey_None) + (gameKey2->ModKeyParam != ImGuiKey_None);

    return keyOneParams > keyTwoParams;
}

void Configs::ReloadSettings()
{
    // Clear Keys
    Input::GameKeys.clear();

    // Combat Arts
    CAFunctions::PerformArraySetup(Input::CombatArtKeys.size());
    for (int i = 0; i < Input::CombatArtKeys.size(); i++)
    {
        if (Input::CombatArtKeys[i].key1 == ImGuiKey_None)
            continue;

        Input::GameKey* newKey = new Input::GameKey(Input::CombatArtKeys[i], Input::EquipmentModifierKey, CAFunctions::TrySelectCombatArt, CAFunctions::CAUsageMode ? Input::RemoveSpecialModeInputs : nullptr, SelectionMenu::SetShowArtMenu, nullptr);
        newKey->PressArgs = new short(i);
        newKey->AltPressArgs = new short(i);
        Input::GameKeys.push_back(newKey);
    }

    // Prosthetic Sets
    ProstheticFunctions::PerformArraySetup(Input::ProstheticSetKeys.size());
    for (int i = 0; i < Input::ProstheticSetKeys.size(); i++)
    {
        if (Input::ProstheticSetKeys[i].key1 == ImGuiKey_None)
            continue;

        Input::GameKey* newKey = new Input::GameKey(Input::ProstheticSetKeys[i], Input::EquipmentModifierKey, ProstheticFunctions::TrySelectProsthetics, SelectionMenu::SetShowProstheticMenu);
        newKey->PressArgs = new short(i);
        newKey->AltPressArgs = new short(i);
        Input::GameKeys.push_back(newKey);
    }

    // Combat Art Key
    Input::GameKey* newKey = new Input::GameKey(Input::CombatArtKey, Input::AddLongPressInput, Input::RemoveLongPressInput);
    newKey->PressArgs = new Input::SekiroInputAction(Input::SIA_CombatArt);
    newKey->ReleaseArgs = new Input::SekiroInputAction(Input::SIA_CombatArt);
    Input::GameKeys.push_back(newKey);

    // Prosthetics
    for (int i = 0; i < 3; i++)
    {
        newKey = new Input::GameKey(Input::ProstheticKeys[i], ProstheticFunctions::SelectProsthetic, ProstheticFunctions::ProstheticUsageMode ? Input::RemoveProstheticInputs : nullptr);
        newKey->PressArgs = new short(i);
        Input::GameKeys.push_back(newKey);
    }

    Input::GameKey* closeKey = new Input::GameKey({ Input::EquipmentModifierKey, ImGuiKey_None }, SelectionMenu::CloseMenu);
    Input::GameKeys.push_back(closeKey);

    // DEBUG
#ifdef HOTKEYS_DEBUG
#endif

    std::sort(Input::GameKeys.begin(), Input::GameKeys.end(), GameKeyCompare);
}
