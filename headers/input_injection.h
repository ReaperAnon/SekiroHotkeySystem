#pragma once

#include <locale>
#include <vector>
#include <Windows.h>
#include <functions.h>
#include <data.h>

namespace Input
{
    enum SekiroInputAction : uint64_t
    {
        SIA_Attack = 1,
        SIA_Block = 4,
        SIA_CombatArt = 5,
        SIA_Jump = 16,
        SIA_SwitchProsthetic = 1024,
        SIA_Dodge = 8192
    };

    bool IsInputActive(SekiroInputAction input);

    void AddSinglePressInput(void* input);

    void AddLongPressInput(void* input);

    void RemoveLongPressInput(void* input);

    void RemoveSpecialModeInputs(void* input);

    uintptr_t ProcessInputsHook(uint64_t inputHandler, uint64_t a2);
}