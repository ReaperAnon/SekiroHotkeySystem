#include <combatarts.h>

int CAFunctions::CAUsageMode = 0;

bool CAFunctions::UseWhileBlocking = false;

bool CAFunctions::UseWhileInAir = false;

bool CAFunctions::UseOnRepeat = false;

int CAFunctions::CombatArtSize = 0;

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
void CAFunctions::PerformArt(bool wasChanged)
{
    Input::SekiroInputAction attackAction = Input::SIA_Attack;
    Input::SekiroInputAction combatArtAction = Input::SIA_CombatArt;
    if (CAUsageMode == 1 || (UseOnRepeat && !wasChanged))
    {
        Input::AddLongPressInput(&combatArtAction);
        return;
    }

    if (UseWhileBlocking)
    {
        if (Input::IsInputActive(Input::SIA_Block))
        {
            Input::AddLongPressInput(&attackAction);
            return;
        }
    }

    if (UseWhileInAir)
    {
        bool isInAir = *reinterpret_cast<byte*>(Hooks::GetInputHandler() + 0x249) & 2;
        if (isInAir)
        {
            Input::AddLongPressInput(&combatArtAction);
            return;
        }
    }
}

void CAFunctions::TrySelectCombatArt(void* idx)
{
    if (!IsCombatArtThreadRunning)
        std::thread(QueueSelectCombatArt, idx).detach();
}

void CAFunctions::QueueSelectCombatArt(void* idx)
{
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
        bool wasChanged = false;
        if (currentMenuID != CombatArts[caIdx])
        {
            wasChanged = true;
            SkillEquipData[16] = 0;
            SkillEquipData[14] = CombatArts[caIdx];
            result = Hooks::SetSkillSlot(1, reinterpret_cast<uint64_t>(SkillEquipData), true) != 0;
            if (result)
                Hooks::PlayUISound(MENU_OPTIONS_CHANGE);
        }
        else Hooks::PlayUISound(MENU_HIGHLIGHT);

        PerformArt(wasChanged);
    }

    return result;
}