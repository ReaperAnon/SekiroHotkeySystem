#include <input_injection.h>

std::vector<Input::SekiroInputAction> longPressInputs;
std::vector<Input::SekiroInputAction> singlePressInputs;

bool Input::IsInputActive(SekiroInputAction input)
{
    byte* inputHandler = Hooks::GetInputHandler();
    if (inputHandler == nullptr)
        return false;

    return *reinterpret_cast<uint64_t *>(inputHandler + 0x10) & input;
}

void Input::AddSinglePressInput(void* input)
{
    singlePressInputs.push_back(*reinterpret_cast<SekiroInputAction*>(input));
}

void Input::AddLongPressInput(void* input)
{
    longPressInputs.push_back(*reinterpret_cast<SekiroInputAction*>(input));
}

void Input::RemoveLongPressInput(void* input)
{
    for (int i = longPressInputs.size() - 1; i >= 0; i--)
    {
        if (longPressInputs[i] == *reinterpret_cast<SekiroInputAction*>(input))
        {
            longPressInputs.erase(longPressInputs.begin() + i);
            return;
        }
    }
}

void Input::RemoveSpecialModeInputs(void* input)
{
    for (int i = longPressInputs.size() - 1; i >= 0; i--)
    {
        if (longPressInputs[i] == SIA_Attack || longPressInputs[i] == SIA_CombatArt)
        {
            longPressInputs.erase(longPressInputs.begin() + i);
            return;
        }
    }
}

uint64_t Input::ProcessInputsHook(uint64_t inputHandler, uint64_t a2)
{
    uint64_t* actionBitfield = reinterpret_cast<uint64_t*>(inputHandler + 0x10);
    uint64_t* actionBlockBitfield = reinterpret_cast<uint64_t*>(inputHandler + 0x18);

    bool injectInputs = longPressInputs.size() > 0;
    if (injectInputs)
    {
        for (const auto& input : longPressInputs)
        {
            //if(input == SIA_CombatArt)
                //*actionBlockBitfield &= ~SIA_Attack; // resets input block, holding key will chain the input

            *actionBitfield |= input;
        }
    }

    injectInputs = singlePressInputs.size() > 0;
    if (injectInputs)
    {
        for (const auto& input : singlePressInputs)
            *actionBitfield |= input;
    }

    // returns a bitfield of all actions that were not blocked
    uint64_t* retValue = reinterpret_cast<uint64_t*>(Hooks::ProcessInputsOrig(inputHandler, a2));

    uint64_t* processedActions = reinterpret_cast<uint64_t*>(inputHandler + 0x20);
    if (injectInputs)
    {
        for (int i = singlePressInputs.size() - 1; i >= 0; i--)
        {
            bool wasProcessed = *processedActions & singlePressInputs[i];
            if (wasProcessed)
                singlePressInputs.erase(singlePressInputs.begin() + i);
        }
    }

    return reinterpret_cast<uint64_t>(retValue);
}
