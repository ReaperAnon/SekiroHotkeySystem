#pragma once

#include <locale>
#include <MinHook.h>
#include <profiles.h>
#include <imgui_main.h>
#include <input_injection.h>

#define FASTCALL __fastcall*

#define MENU_OPEN					0x29, 0x7530
#define MENU_CLOSE					0x29, 0x7594
#define MENU_SWITCH					0x29, 0x1F4
#define MENU_SUBLIST_OPEN			0x29, 0x3E8
#define MENU_SUBLIST_SELECT_CLOSE	0x29, 0x3FC
#define MENU_OPTIONS_OPEN			0x29, 0x64
#define MENU_OPTIONS_CHANGE			0x29, 0x2BC
#define MENU_POPUP_OPEN				0x29, 0x00
#define MENU_BLOCKED				0x29, 0x00
#define MENU_HIGHLIGHT				0x2B, 0x2710

namespace Hooks
{
	// Called Functions
	typedef uint64_t(FASTCALL tSetSkillSlot)(int equipSlot, uint64_t dataPointer, bool ignoreEquipLock);
		extern tSetSkillSlot SetSkillSlot;

	typedef void* (FASTCALL tSetEquippedProsthetic)(uint64_t* characterBase, unsigned zero, unsigned prostheticNum);
		extern tSetEquippedProsthetic SetEquippedProsthetic;

	typedef void (FASTCALL tPlayUISound)(signed int arg1, int arg2);
		extern tPlayUISound PlayUISound;

	typedef const char* (FASTCALL tGetItemName)(uint64_t msgRepository, unsigned realID);
		extern tGetItemName GetItemName;

	typedef uint64_t(FASTCALL tGetItemMenuID)(uint64_t EquipInventoryDataPtr, unsigned* realID);
		extern tGetItemMenuID GetMenuID;


	// Hooked Functions
	typedef uint64_t(FASTCALL tProcessInputs)(uint64_t inputProcessBase, uint64_t arg2);
		extern tProcessInputs ProcessInputs;
		extern tProcessInputs ProcessInputsOrig;

	typedef uint64_t(FASTCALL tDeleteSave)(uint64_t arg1, uint64_t arg2, unsigned saveSlot);
		extern tDeleteSave DeleteSave;
		extern tDeleteSave DeleteSaveOrig;

	typedef bool (FASTCALL tIsMenuInputActive)(uint64_t arg1, float arg2);
		extern tIsMenuInputActive IsMenuInputActive;
		extern tIsMenuInputActive IsMenuInputActiveOrig;

	typedef bool (FASTCALL tIsMenuInputActiveNoArgs)();
		extern tIsMenuInputActiveNoArgs IsMenuInputActiveNoArgs;
		extern tIsMenuInputActiveNoArgs IsMenuInputActiveNoArgsOrig;

	typedef bool (FASTCALL tParseGamepadInput)(uint64_t padDevicePtr);
		extern tParseGamepadInput ParseGamepadInput;
		extern tParseGamepadInput ParseGamepadInputOrig;

	// Unused Functions
	// typedef uint64_t (FASTCALL tGetSlotID)(int slotIdx);
	// 	extern tGetSlotID GetSlotID; // = reinterpret_cast<tGetSlotID>(0x1409CB5D0);

	// typedef int* (FASTCALL tGetEquippedInSlot)(uint64_t base, unsigned* equipID, int slotID);
	// 	extern tGetEquippedInSlot GetEquippedInSlot; // = reinterpret_cast<tGetEquippedInSlot>(0x1409CBE40);

	void HookFunctions();
}