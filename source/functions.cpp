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

// typedef uint64_t(FASTCALL tGetIconID)(uint64_t EquipInventoryDataPtrPlus0x10, unsigned* realID);
	Hooks::tGetIconID Hooks::GetIconID = reinterpret_cast<tGetIconID>(0x140C3D680);


// Hooked Functions
// typedef uint64_t(FASTCALL tProcessInputs)(uint64_t inputProcessBase, uint64_t arg2);
	Hooks::tProcessInputs Hooks::ProcessInputs = reinterpret_cast<tProcessInputs>(0x140B2C190);
	Hooks::tProcessInputs Hooks::ProcessInputsOrig = nullptr;
		
// typedef uint64_t(FASTCALL tDeleteSave)(uint64_t arg1, uint64_t arg2, unsigned saveSlot);
	Hooks::tDeleteSave Hooks::DeleteSave = reinterpret_cast<tDeleteSave>(0x140D38DB0);
	Hooks::tDeleteSave Hooks::DeleteSaveOrig = nullptr;

// typedef bool (FASTCALL tIsMenuInputActive)(uint64_t arg1, float arg2);
	Hooks::tIsMenuInputActive Hooks::RestrictUpdateUI = reinterpret_cast<tIsMenuInputActive>(0x140D884F0);
	Hooks::tIsMenuInputActive Hooks::RestrictUpdateUIOrig = nullptr;

// typedef bool (FASTCALL tIsMenuInputActiveNoArgs)();
	Hooks::tIsMenuInputActiveNoArgs Hooks::IsMenuModeActive = reinterpret_cast<tIsMenuInputActiveNoArgs>(0x1408BC400);
	Hooks::tIsMenuInputActiveNoArgs Hooks::IsMenuModeActiveOrig = nullptr;

// typedef bool (FASTCALL tParseGamepadInput)(uint64_t padDevicePtr);
	Hooks::tParseGamepadInput Hooks::ParseGamepadInput = reinterpret_cast<tParseGamepadInput>(0x141A8D3E0);
	Hooks::tParseGamepadInput Hooks::ParseGamepadInputOrig = nullptr;

// typedef double (FASTCALL tProcessAnalogInput)(uint64_t PadDevicePtr, int index);
	Hooks::tProcessAnalogInput Hooks::ProcessAnalogInput = reinterpret_cast<tProcessAnalogInput>(0x141A8CBC0);
	Hooks::tProcessAnalogInput Hooks::ProcessAnalogInputOrig = nullptr;

// typedef uint64_t(FASTCALL tGetGameSpeed)(uint64_t arg1, uint64_t timePerFrame);
	Hooks::tGetGameSpeed Hooks::GetGameSpeed = reinterpret_cast<tGetGameSpeed>(0x1411DF900);
	Hooks::tGetGameSpeed Hooks::GetGameSpeedOrig = nullptr;


void Hooks::HookFunctions()
{
	MH_CreateHook(RestrictUpdateUI, ImGui::RestrictUpdateUIHook, (LPVOID*)&RestrictUpdateUIOrig)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "\nIsMenuInputActive Hooked\n" : std::cout << "\nIsMenuInputActive Hook Failed\n"
#endif
		;
	MH_EnableHook(RestrictUpdateUI)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "IsMenuInputActive Hook Activated\n" : std::cout << "IsMenuInputActive Hook Activation Failed\n"
#endif
		;


	MH_CreateHook(IsMenuModeActive, ImGui::IsMenuModeActiveHook, (LPVOID*)&IsMenuModeActiveOrig)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "\nIsMenuModeActive Hooked\n" : std::cout << "\nIsMenuModeActive Hook Failed\n"
#endif
		;
	MH_EnableHook(IsMenuModeActive)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "IsMenuModeActive Hook Activated\n" : std::cout << "IsMenuModeActive Hook Activation Failed\n"
#endif
		;


	MH_CreateHook(ParseGamepadInput, ImGui::CheckRestrictInputsGamepad, (LPVOID*)&ParseGamepadInputOrig)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "\nParseGamepadInput Hooked\n" : std::cout << "\nParseGamepadInput Hook Failed\n"
#endif
		;
	MH_EnableHook(ParseGamepadInput)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "ParseGamepadInput Hook Activated\n" : std::cout << "ParseGamepadInput Hook Activation Failed\n"
#endif
		;


	MH_CreateHook(ProcessInputs, Input::ProcessInputsHook, (LPVOID*)&ProcessInputsOrig)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "\nProcessInputs Hooked\n" : std::cout << "\nProcessInputs Hook Failed\n"
#endif
		;
	MH_EnableHook(ProcessInputs)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "ProcessInputs Hook Activated\n" : std::cout << "ProcessInputs Hook Activation Failed\n"
#endif
		;


	MH_CreateHook(ProcessAnalogInput, ImGui::ProcessAnalogInputHook, (LPVOID*)&ProcessAnalogInputOrig)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "\nProcessAnalogInput Hooked\n" : std::cout << "\nProcessAnalogInput Hook Failed\n"
#endif
		;
	MH_EnableHook(ProcessAnalogInput)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "ProcessAnalogInput Hook Activated\n" : std::cout << "ProcessAnalogInput Hook Activation Failed\n"
#endif
		;


	MH_CreateHook(GetGameSpeed, ImGui::GetGameSpeedHook, (LPVOID*)&GetGameSpeedOrig)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "\nGetGameSpeed Hooked\n" : std::cout << "\nGetGameSpeed Hook Failed\n"
#endif
		;
	MH_EnableHook(GetGameSpeed)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "GetGameSpeed Hook Activated\n" : std::cout << "GetGameSpeed Hook Activation Failed\n"
#endif
		;


	MH_CreateHook(DeleteSave, Profiles::DeleteSaveHook, (LPVOID*)&DeleteSaveOrig)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "\nDeleteSave Hooked\n" : std::cout << "\nDeleteSave Hook Failed\n"
#endif
		;
	MH_EnableHook(DeleteSave)
#ifdef HOTKEYS_DEBUG
		== MH_OK ? std::cout << "DeleteSave Hook Activated\n" : std::cout << "DeleteSave Hook Activation Failed\n"
#endif
		;
}