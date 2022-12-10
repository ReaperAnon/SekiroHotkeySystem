#include <configs.h>

void Configs::ReadConfigFile()
{
    WCHAR inputBuffer[50];
    WCHAR configName[] = L".\\configs\\sekirohotkeys.ini";

    Input::CombatArtKeys.clear();
    Input::ProstheticSetKeys.clear();

    short idx = 0;
    int keyValue1, keyValue2;
    do
    {
        Input::ImGuiKeySet keySet;
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
        Input::ImGuiKeySet keySet;
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

    GetPrivateProfileString(L"Menu", L"configScale", L"1.1", inputBuffer, 50, configName);
    ConfigMenu::ConfigScale = std::stof(inputBuffer);
    GetPrivateProfileString(L"Menu", L"widgetScale", L"1.4", inputBuffer, 50, configName);
    WidgetMenu::WidgetSettings.widgetScale = std::stof(inputBuffer);
    WidgetMenu::WidgetSettings.showHotkeys = GetPrivateProfileInt(L"Menu", L"widgetDisplayKey", 1, configName);
    WidgetMenu::WidgetSettings.widgetMode = GetPrivateProfileInt(L"Menu", L"widgetMode", 0, configName);
    WidgetMenu::WidgetSettings.widgetPosition = GetPrivateProfileInt(L"Menu", L"widgetPosition", 0, configName);
    WidgetMenu::WidgetSettings.widgetColor = GetPrivateProfileInt(L"Menu", L"widgetColor", 0, configName);

    CAFunctions::CAUsageMode = GetPrivateProfileInt(L"General", L"UsageMode", 3, configName);

    Input::ImGuiKeySet keySet;
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

    Input::GameKey* openKeyGamepad = new Input::GameKey({ ImGuiKey_GamepadBack, ImGuiKey_None }, ConfigMenu::OpenConfigMenu);
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
    configText += "\nCombatArtKey1=";
    configText += std::to_string(Input::CombatArtKey.key1);
    configText += "\nCombatArtKey2=";
    configText += std::to_string(Input::CombatArtKey.key2);

    // Combat Arts
    configText += "\n\n[Combat Arts]";
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

        if (CAFunctions::CAUsageMode == 1 || CAFunctions::CAUsageMode == 2)
        {
            Input::GameKey* newKey = new Input::GameKey(Input::CombatArtKeys[i], CAFunctions::TrySelectCombatArt, Input::RemoveLongPressInput);
            newKey->PressArgs = new short(i);
            newKey->ReleaseArgs = new Input::SekiroInputAction(Input::SIA_CombatArt);
            Input::GameKeys.push_back(newKey);
        }
        else if (CAFunctions::CAUsageMode == 3)
        {
            Input::GameKey* newKey = new Input::GameKey(Input::CombatArtKeys[i], CAFunctions::TrySelectCombatArt, Input::RemoveLongPressInput);
            newKey->PressArgs = new short(i);
            newKey->ReleaseArgs = new Input::SekiroInputAction(Input::SIA_Attack);
            Input::GameKeys.push_back(newKey);
        }
        else if (CAFunctions::CAUsageMode == 4)
        {
            Input::GameKey* newKey = new Input::GameKey(Input::CombatArtKeys[i], CAFunctions::TrySelectCombatArt, Input::RemoveSpecialModeInputs);
            newKey->PressArgs = new short(i);
            Input::GameKeys.push_back(newKey);
        }
        else
        {
            Input::GameKey* newKey = new Input::GameKey(Input::CombatArtKeys[i], CAFunctions::TrySelectCombatArt);
            newKey->PressArgs = new short(i);
            Input::GameKeys.push_back(newKey);
        }
    }

    // Prosthetic Sets
    ProstheticFunctions::PerformArraySetup(Input::ProstheticSetKeys.size());
    for (int i = 0; i < Input::ProstheticSetKeys.size(); i++)
    {
        if (Input::ProstheticSetKeys[i].key1 == ImGuiKey_None)
            continue;

        Input::GameKey* newKey = new Input::GameKey(Input::ProstheticSetKeys[i], ProstheticFunctions::TrySelectProsthetics);
        newKey->PressArgs = new short(i);
        Input::GameKeys.push_back(newKey);
    }

    // Combat Art Key
    Input::GameKey* newKey = new Input::GameKey(Input::CombatArtKey, Input::AddLongPressInput, Input::RemoveLongPressInput);
    newKey->PressArgs = new Input::SekiroInputAction(Input::SIA_CombatArt);
    newKey->ReleaseArgs = new Input::SekiroInputAction(Input::SIA_CombatArt);
    Input::GameKeys.push_back(newKey);

    // Prosthetics
    newKey = new Input::GameKey(Input::ProstheticKeys[0], ProstheticFunctions::SelectProsthetic);
    newKey->PressArgs = new short(0);
    Input::GameKeys.push_back(newKey);

    newKey = new Input::GameKey(Input::ProstheticKeys[1], ProstheticFunctions::SelectProsthetic);
    newKey->PressArgs = new short(1);
    Input::GameKeys.push_back(newKey);

    newKey = new Input::GameKey(Input::ProstheticKeys[2], ProstheticFunctions::SelectProsthetic);
    newKey->PressArgs = new short(2);
    Input::GameKeys.push_back(newKey);
}
