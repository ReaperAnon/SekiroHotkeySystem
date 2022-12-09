#include <combatarts.h>

int CAFunctions::CAUsageMode;

int CAFunctions::CombatArtSize;

std::vector<unsigned> CAFunctions::CombatArts;

bool IsCombatArtThreadRunning = false;

unsigned SkillEquipData[17];


// Array Manipulation
void CAFunctions::PerformArraySetup(const unsigned length)
{
    CombatArts.resize(length);
}

bool CAFunctions::IsArrayEmpty()
{
    unsigned result = 0;
    for (const auto& entry : CombatArts)
        result += entry;

    return result == 0;
}

void CAFunctions::ClearEquipmentSlots()
{
    size_t length = CombatArts.size();
    CombatArts.clear();
    CombatArts.resize(length);
    WidgetMenu::CombatArtNames.clear();
}


// Combat Art Functionality
void CAFunctions::PerformArt()
{
    Input::SekiroInputAction attackAction = Input::SIA_Attack;
    Input::SekiroInputAction combatArtAction = Input::SIA_CombatArt;
    if (CAUsageMode == 4)
    {
        if (Input::IsInputActive(Input::SIA_Block))
        {
            Input::AddLongPressInput(&attackAction);
        }
        else
        {
            bool isInAir = *reinterpret_cast<byte*>(Hooks::GetInputHandler() + 0x249) & 2;
            if (isInAir)
                Input::AddLongPressInput(&combatArtAction);
        }
    }
    else if (CAUsageMode == 3)
    {
        if (Input::IsInputActive(Input::SIA_Block))
            Input::AddLongPressInput(&attackAction);
    }
    else if (CAUsageMode == 2)
    {
        bool isInAir = *reinterpret_cast<byte*>(Hooks::GetInputHandler() + 0x249) & 2;
        if (isInAir)
            Input::AddLongPressInput(&combatArtAction);
    }
    else if (CAUsageMode == 1)
    {
        Input::AddLongPressInput(&combatArtAction);
    }
}

void CAFunctions::TrySelectCombatArt(void* idx)
{
    if (!IsCombatArtThreadRunning)
        _beginthread(QueueSelectCombatArt, 0, idx);
}

void CAFunctions::QueueSelectCombatArt(void* idx)
{
    if (Hooks::GetCharacterBase() == 0)
        return;

    IsCombatArtThreadRunning = true;
    while (!SelectCombatArt(idx))
        Sleep(20);

    IsCombatArtThreadRunning = false;
}

bool CAFunctions::SelectCombatArt(void* idx)
{
    bool result = true;
    uint64_t skillBase = Hooks::GetSkillBase();

    if (skillBase == 0) // exit if not in-game
        return false;

    short caIdx = *reinterpret_cast<short*>(idx);
    unsigned currentMenuID = *reinterpret_cast<unsigned*>(skillBase + 0x28);
    if (Hooks::IsInMenu() || CombatArts[caIdx] == 0)
    {
        CombatArts[caIdx] = currentMenuID;
        Hooks::PlayUISound(MENU_SUBLIST_SELECT_CLOSE);
        WidgetMenu::UpdateCombatArtNames();
    }
    else
    {
        if (currentMenuID != CombatArts[caIdx])
        {
            SkillEquipData[16] = 0;
            SkillEquipData[14] = CombatArts[caIdx];
            result = Hooks::SetSkillSlot(1, reinterpret_cast<uint64_t>(SkillEquipData), true) != 0;
            if (result)
            {
                PerformArt();
                Hooks::PlayUISound(MENU_OPTIONS_CHANGE);
            }
        }
        else
        {
            PerformArt();
            Hooks::PlayUISound(MENU_HIGHLIGHT);
        }
    }

    return result;
}