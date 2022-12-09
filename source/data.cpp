#include <data.h>

const bool* const IsInMenuVar = reinterpret_cast<bool *>(0x143D8990C);
const bool* const IsInCutsceneVar = reinterpret_cast<bool *>(0x143D7ACBE);
const bool* const IsInConversationVar = reinterpret_cast<bool *>(0x143D8A050);


bool Hooks::IsInMenu()
{
	return *IsInMenuVar;
}

bool Hooks::IsHUDVisible()
{
    byte* uiValues = GetUIValuesBase();
    if (uiValues == nullptr)
        return false;

    bool isItemVisible = *reinterpret_cast<bool *>(uiValues + 0xE8);
    bool isHealthVisible = *reinterpret_cast<bool *>(uiValues + 0x1C);
	return !(*IsInMenuVar || *IsInCutsceneVar || *IsInConversationVar) && isItemVisible && isHealthVisible;
}

byte* SaveSlotBase = reinterpret_cast<byte*>(0x143D5C0D0);
int Hooks::GetSaveSlotIdx()
{
    return *reinterpret_cast<int*>(ResolvePointerChain(SaveSlotBase, { 0xAE4 }));
}

byte* Hooks::ResolvePointerChain(byte* baseAddress, std::vector<int> offsets)
{
    byte* retAddress = baseAddress;
    for (const auto& offset : offsets)
    {
        if(retAddress)
            ReadProcessMemory(GetCurrentProcess(), retAddress, &retAddress, sizeof(retAddress), 0);

        if (retAddress == nullptr)
            break;

        retAddress += offset;
    }

    return reinterpret_cast<uint64_t>(retAddress) < 0x140000000 ? nullptr : retAddress;
}

byte* SkillBase = reinterpret_cast<byte *>(0x143D5AAC0);
uint64_t Hooks::GetSkillBase()
{
    return reinterpret_cast<uint64_t>(ResolvePointerChain(SkillBase, { 0x08, 0x278 }));
}

byte* CharacterBase = reinterpret_cast<byte *>(0x143D7A1E0);
uint64_t Hooks::GetCharacterBase()
{
    return reinterpret_cast<uint64_t>(ResolvePointerChain(CharacterBase, { 0x88, 0x1F10, 0x10, 0xF8, 0x10, 0x18, 0x00 }));
}

byte* InputBase = reinterpret_cast<byte *>(0x143D7A1E0);
byte* Hooks::GetInputHandler()
{
    return ResolvePointerChain(InputBase, { 0x88, 0x1FF8, 0x80, 0x00 });
}

byte* UIBase = reinterpret_cast<byte *>(0x143D67410);
byte* Hooks::GetUIValuesBase()
{
    return ResolvePointerChain(UIBase, { 0x1E0 });
}

uint64_t Hooks::GetPlayerGameData()
{
    uint64_t GameData = *reinterpret_cast<uint64_t*>(0x143D5AAC0);
    if (!GameData)
        return 0;

    uint64_t PlayerGameData = *reinterpret_cast<uint64_t*>(GameData + 0x8);
    if (!PlayerGameData)
        return 0;

    return PlayerGameData;
}

uint64_t Hooks::GetEquipInventoryData()
{
    uint64_t PlayerGameData = GetPlayerGameData();
    if (!PlayerGameData)
        return 0;

    uint64_t EquipInventoryData = *reinterpret_cast<uint64_t*>(PlayerGameData + 0x5B0);
    if (!EquipInventoryData)
        return 0;

    return EquipInventoryData;
}

bool Hooks::IsGameLoaded()
{
    uint64_t MenuEntity = *reinterpret_cast<uint64_t*>(0x143D67408);
    if (!MenuEntity)
        return 0;

    uint64_t WorldEntity = *reinterpret_cast<uint64_t*>(0x143D7A1E0);

    return *reinterpret_cast<bool*>(MenuEntity + 0x248) && WorldEntity;
}
