#include <prosthetics.h>

int SkillEquipData[17];

int ProstheticFunctions::ProstheticSetSize;

std::vector<ProstheticFunctions::ProstheticSet> ProstheticFunctions::ProstheticSets;

bool IsProstheticThreadRunning = false;


// Array Manipulation
void ProstheticFunctions::PerformArraySetup(const int length)
{
    ProstheticSets.resize(length);
}

void ProstheticFunctions::ClearEquipmentSlots()
{
    size_t length = ProstheticSets.size();
    ProstheticSets.clear();
    ProstheticSets.resize(length);
}


// Prosthetic Functions
bool ProstheticFunctions::IsIDInvalid(unsigned menuID)
{
    return menuID == 0x100 || menuID == 0x102 || menuID == 0x104;
}

void ProstheticFunctions::TrySelectProsthetics(void* idx)
{
    if (!IsProstheticThreadRunning)
        _beginthread(QueueSelectProsthetics, 0, idx);
}

void ProstheticFunctions::QueueSelectProsthetics(void* idx)
{
    bool wasChanged = false;
    IsProstheticThreadRunning = true;
    while (!SelectProstheticGroup(idx, &wasChanged))
        Sleep(100);

    if (wasChanged)
    {
        Sleep(50);
        short currIdx = *reinterpret_cast<byte*>(Hooks::GetSkillBase() + 0xA4);
        currIdx -= 1;
        if (currIdx < 0)
            currIdx = 2;

        Hooks::SetEquippedProsthetic(*reinterpret_cast<uint64_t**>(Hooks::GetCharacterBase() + 0x10), 0, currIdx);
        Input::SekiroInputAction switchProsthetic = Input::SIA_SwitchProsthetic;
        Input::AddSinglePressInput(&switchProsthetic);
    }

    IsProstheticThreadRunning = false;
}

bool ProstheticFunctions::SelectProstheticGroup(void* idx, bool* wasChanged)
{
    bool result = true;
    uint64_t skillBase = Hooks::GetSkillBase();

    if (skillBase == 0) // exit if not in-game
        return false;

    short prostheticIdx = *reinterpret_cast<short*>(idx);
    unsigned currentMenuID1 = *reinterpret_cast<unsigned*>(skillBase + 0x24);
    unsigned currentMenuID2 = *reinterpret_cast<unsigned*>(skillBase + 0x2C);
    unsigned currentMenuID3 = *reinterpret_cast<unsigned*>(skillBase + 0x34);

    if (Hooks::IsInMenu() || ProstheticSets[prostheticIdx].slotOne == 0)
    {
        ProstheticSets[prostheticIdx].slotOne = currentMenuID1;
        ProstheticSets[prostheticIdx].slotTwo = currentMenuID2;
        ProstheticSets[prostheticIdx].slotThree = currentMenuID3;
        Hooks::PlayUISound(MENU_SUBLIST_SELECT_CLOSE);
        return true;
    }
    else
    {
        SkillEquipData[16] = 0;
        if (currentMenuID1 != ProstheticSets[prostheticIdx].slotOne)
        {
            SkillEquipData[14] = ProstheticSets[prostheticIdx].slotOne;
            result &= Hooks::SetSkillSlot(0, reinterpret_cast<uint64_t>(SkillEquipData), true) != 0;
            *wasChanged = true;
        }
        if (currentMenuID2 != ProstheticSets[prostheticIdx].slotTwo)
        {
            SkillEquipData[14] = ProstheticSets[prostheticIdx].slotTwo;
            result &= Hooks::SetSkillSlot(2, reinterpret_cast<uint64_t>(SkillEquipData), true) != 0;
            *wasChanged = true;
        }
        if (currentMenuID3 != ProstheticSets[prostheticIdx].slotThree)
        {
            SkillEquipData[14] = ProstheticSets[prostheticIdx].slotThree;
            result &= Hooks::SetSkillSlot(4, reinterpret_cast<uint64_t>(SkillEquipData), true) != 0;
            *wasChanged = true;
        }

        if (!result)
            return false;

        if (*wasChanged)
        {
            Hooks::PlayUISound(MENU_OPTIONS_CHANGE);
        }
        else
        {
            Hooks::PlayUISound(MENU_BLOCKED);
        }
    }

    return result;
}

void ProstheticFunctions::SelectProsthetic(void* idx)
{
    uint64_t skillBase = Hooks::GetSkillBase();
    if (Hooks::IsInMenu() || skillBase == 0) // exit if not in-game
        return;

    short index = *reinterpret_cast<short*>(idx);
    unsigned currentMenuID1 = *reinterpret_cast<unsigned*>(skillBase + 0x24);
    unsigned currentMenuID2 = *reinterpret_cast<unsigned*>(skillBase + 0x2C);
    unsigned currentMenuID3 = *reinterpret_cast<unsigned*>(skillBase + 0x34);

    if (index == 0 && IsIDInvalid(currentMenuID1))
        return;
    if (index == 1 && IsIDInvalid(currentMenuID2))
        return;
    if (index == 2 && IsIDInvalid(currentMenuID3))
        return;

    index -= 1;
    if (index < 0)
        index = 2;

    uint64_t characterBase = Hooks::GetCharacterBase();
    Hooks::SetEquippedProsthetic(*reinterpret_cast<uint64_t**>(characterBase + 0x10), 0, index);
    Input::SekiroInputAction switchProsthetic = Input::SIA_SwitchProsthetic;
    Input::AddSinglePressInput(&switchProsthetic);
}