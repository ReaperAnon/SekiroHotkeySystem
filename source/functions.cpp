#include <functions.h>

// Called Functions
// typedef uint64_t (FASTCALL tSetSkillSlot)(int equipSlot, uint64_t dataPointer, bool ignoreEquipLock);
	Hooks::tSetSkillSlot Hooks::SetSkillSlot = reinterpret_cast<tSetSkillSlot>(0x140D592F0);

// typedef void* (FASTCALL tSetEquippedProsthetic)(uint64_t* characterBase, unsigned zero, unsigned prostheticNum);
	Hooks::tSetEquippedProsthetic Hooks::SetEquippedProsthetic = reinterpret_cast<tSetEquippedProsthetic>(0x140A26150);

// typedef void (FASTCALL tPlayUISound)(signed int arg1, int arg2);
	Hooks::tPlayUISound Hooks::PlayUISound = reinterpret_cast<tPlayUISound>(0x1408CE960);

// typedef const char* (FASTCALL tGetItemName)(uint64_t msgRepository, unsigned realID);
	Hooks::tGetItemName Hooks::GetItemName = reinterpret_cast<tGetItemName>(0x1410AA340);

// typedef uint64_t(FASTCALL tGetItemMenuID)(uint64_t EquipInventoryDataPtr, unsigned* realID);
	Hooks::tGetItemMenuID Hooks::GetMenuID = reinterpret_cast<tGetItemMenuID>(0x140C3D680);


// Hooked Functions
// typedef uint64_t(FASTCALL tProcessInputs)(uint64_t inputProcessBase, uint64_t arg2);
	Hooks::tProcessInputs Hooks::ProcessInputs = reinterpret_cast<tProcessInputs>(0x140B2C190);
	Hooks::tProcessInputs Hooks::ProcessInputsOrig;
		
// typedef uint64_t(FASTCALL tDeleteSave)(uint64_t arg1, uint64_t arg2, unsigned saveSlot);
	Hooks::tDeleteSave Hooks::DeleteSave = reinterpret_cast<tDeleteSave>(0x140D38DB0);
	Hooks::tDeleteSave Hooks::DeleteSaveOrig;

// typedef bool (FASTCALL tIsMenuInputActive)(uint64_t arg1, float arg2);
	Hooks::tIsMenuInputActive Hooks::IsMenuInputActive = reinterpret_cast<tIsMenuInputActive>(0x140D884F0);
	Hooks::tIsMenuInputActive Hooks::IsMenuInputActiveOrig;

// typedef bool (FASTCALL tIsMenuInputActiveNoArgs)();
	Hooks::tIsMenuInputActiveNoArgs Hooks::IsMenuInputActiveNoArgs = reinterpret_cast<tIsMenuInputActiveNoArgs>(0x1408BC400);
	Hooks::tIsMenuInputActiveNoArgs Hooks::IsMenuInputActiveNoArgsOrig;

	Hooks::tParseGamepadInput Hooks::ParseGamepadInput = reinterpret_cast<tParseGamepadInput>(0x141A8D3E0);
	Hooks::tParseGamepadInput Hooks::ParseGamepadInputOrig;


void Hooks::HookFunctions()
{
	MH_CreateHook(IsMenuInputActive, ImGui::CheckRestrictInputs, (LPVOID*)&IsMenuInputActiveOrig);
	MH_EnableHook(IsMenuInputActive);

	MH_CreateHook(IsMenuInputActiveNoArgs, ImGui::CheckRestrictInputsNoArgs, (LPVOID*)&IsMenuInputActiveNoArgsOrig);
	MH_EnableHook(IsMenuInputActiveNoArgs);

	MH_CreateHook(ParseGamepadInput, ImGui::CheckRestrictInputsGamepad, (LPVOID*)&ParseGamepadInputOrig);
	MH_EnableHook(ParseGamepadInput);

	MH_CreateHook(ProcessInputs, Input::ProcessInputsHook, (LPVOID*)&ProcessInputsOrig);
	MH_EnableHook(ProcessInputs);

	MH_CreateHook(DeleteSave, Profiles::DeleteSaveHook, (LPVOID*)&DeleteSaveOrig);
	MH_EnableHook(DeleteSave);
}