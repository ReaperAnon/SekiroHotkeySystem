#include <prosthetics.h>

int SkillEquipData[17];

int ProstheticFunctions::ProstheticUsageMode = 0;

bool ProstheticFunctions::UseWhileBlocking = false;

bool ProstheticFunctions::UseWhileInAir = false;

bool ProstheticFunctions::UseOnRepeat = false;

int ProstheticFunctions::ProstheticSetSize = 0;

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
void ProstheticFunctions::PerformProstheticAttack(bool wasChanged)
{
    Input::SekiroInputAction prostheticAction = Input::SIA_UseProsthetic;
    if (ProstheticUsageMode == 1 || (UseOnRepeat && !wasChanged))
    {
        Input::AddLongPressInput(&prostheticAction);
        return;
    }

    if (UseWhileBlocking)
    {
        if (Input::IsInputActive(Input::SIA_Block))
        {
            Input::AddLongPressInput(&prostheticAction);
            return;
        }
    }

    if (UseWhileInAir)
    {
        bool isInAir = *reinterpret_cast<byte*>(Hooks::GetInputHandler() + 0x249) & 2;
        if (isInAir)
        {
            Input::AddLongPressInput(&prostheticAction);
            return;
        }
    }
}

bool ProstheticFunctions::IsIDInvalid(unsigned menuID)
{
    return menuID == 0x100 || menuID == 0x102 || menuID == 0x104;
}

void ProstheticFunctions::TrySelectProsthetics(void* idx)
{
    if (!IsProstheticThreadRunning)
        std::thread(QueueSelectProsthetics, idx).detach();
}

void ProstheticFunctions::QueueSelectProsthetics(void* idx)
{
    bool wasChanged = false;
    IsProstheticThreadRunning = true;
    while (!SelectProstheticGroup(idx, &wasChanged))
        Sleep(100);

    if (wasChanged)
    {
        SetIndexToNonEmptySlot(idx);
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
    bool wasChanged = false;
    uint64_t skillBase = Hooks::GetSkillBase();
    if (Hooks::IsInMenu() || skillBase == 0) // exit if not in-game
        return;

    short index = *reinterpret_cast<short*>(idx);
    short currIndex = *reinterpret_cast<short*>(skillBase + 0xA4);
    if (currIndex != index)
    {
        unsigned currentMenuID1 = *reinterpret_cast<unsigned*>(skillBase + 0x24);
        unsigned currentMenuID2 = *reinterpret_cast<unsigned*>(skillBase + 0x2C);
        unsigned currentMenuID3 = *reinterpret_cast<unsigned*>(skillBase + 0x34);

        if (index == 0 && IsIDInvalid(currentMenuID1))
            return;
        if (index == 1 && IsIDInvalid(currentMenuID2))
            return;
        if (index == 2 && IsIDInvalid(currentMenuID3))
            return;

        uint64_t characterBase = Hooks::GetCharacterBase();
        Hooks::SetEquippedProsthetic(*reinterpret_cast<uint64_t**>(characterBase + 0x10), 0, index);
        wasChanged = true;
    }

    PerformProstheticAttack(wasChanged);
}

void ProstheticFunctions::SetIndexToNonEmptySlot(void* idx)
{
    short prostheticIdx = *reinterpret_cast<short*>(idx);
    short* currIdx = reinterpret_cast<short*>(Hooks::GetSkillBase() + 0xA4);

    short initialIdx = *currIdx, nextIdx = initialIdx;
    bool isEmptySlot = true, isFirstIteration = true;
    while (isEmptySlot)
    {
        isEmptySlot = false;
        isEmptySlot |= nextIdx == 0 && PROSTHETIC_EMPTY_SLOT == ProstheticSets[prostheticIdx].slotOne;
        isEmptySlot |= nextIdx == 1 && PROSTHETIC_EMPTY_SLOT == ProstheticSets[prostheticIdx].slotTwo;
        isEmptySlot |= nextIdx == 2 && PROSTHETIC_EMPTY_SLOT == ProstheticSets[prostheticIdx].slotThree;

        if (isFirstIteration)
        {
            isFirstIteration = false;
        }
        else if (initialIdx == nextIdx)
        {
            break;
        }

        nextIdx += isEmptySlot;

        if (nextIdx > 2)
            nextIdx = 0;
    }
    *currIdx = nextIdx;
}