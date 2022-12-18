#pragma once

#include <vector>
#include <thread>
#include <data.h>
#include <functions.h>

namespace ProstheticFunctions
{
    struct ProstheticSet
    {
        unsigned slotOne;
        unsigned slotTwo;
        unsigned slotThree;
    };

    extern int ProstheticUsageMode;

    extern bool UseWhileBlocking;

    extern bool UseWhileInAir;

    extern bool UseOnRepeat;

    extern int ProstheticSetSize;

	extern std::vector<ProstheticSet> ProstheticSets;

    void PerformArraySetup(const int length);

    void ClearEquipmentSlots();

    void PerformProstheticAttack(bool wasChanged);

    bool IsIDInvalid(unsigned menuID);

    void TrySelectProsthetics(void* idx);

    void QueueSelectProsthetics(void* idx);

    bool SelectProstheticGroup(void* idx, bool* wasChanged);

    void SelectProsthetic(void* idx);
}