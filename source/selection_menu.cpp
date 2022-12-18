#include <selection_menu.h>


bool SelectionMenu::IsMenuOpen = false;

bool SelectionMenu::ProstheticMode = false;

bool IsSelectionInitialized = false;

short WorkingIndex = 0;

std::vector<SelectionMenu::EquipEntry> SelectionMenu::EquipEntries;


void SelectionMenu::SetShowArtMenu(void *idx)
{
	if (IsMenuOpen)
		return;

	WorkingIndex = *reinterpret_cast<short*>(idx);
	ProstheticMode = false;
	IsMenuOpen = true;
}

void SelectionMenu::SetShowProstheticMenu(void *idx)
{
	if (IsMenuOpen)
		return;

	WorkingIndex = *reinterpret_cast<short*>(idx);
	ProstheticMode = true;
	IsMenuOpen = true;
}

void SelectionMenu::CloseMenu(void *)
{
	IsMenuOpen = false;
	IsSelectionInitialized = false;
}

bool SelectionMenu::ShowSelectionMenu()
{
	if (Hooks::IsInMenu())
	{
		IsMenuOpen = false;
		return false;
	}

	if (!IsMenuOpen)
		return false;

	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f), NULL, ImVec2(0.5f, 0.5f));
	if (!ImGui::Begin("selection base", nullptr, windowFlags))
		return false;

	ImGui::OpenPopup("selection popup");
	windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f), NULL, ImVec2(0.5f, 0.5f));
	if (!ImGui::BeginPopup("selection popup", windowFlags))
		return false;

	static int selectedNum = 0;
	static bool selectedProsthetics[100];
	if (!IsSelectionInitialized)
	{
		selectedNum = 0;
		IsSelectionInitialized = true;
		memset(selectedProsthetics, 0, sizeof(selectedProsthetics));

		ProstheticMode ? Hooks::GetProstheticEquipSet() : Hooks::GetCombatArtEquipSet(WorkingIndex);
		if(!ProstheticMode)
			std::sort(EquipEntries.begin(), EquipEntries.end(), [](EquipEntry a, EquipEntry b) { return a.equipName[0] < b.equipName[0]; });

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	}

	for (int i=0; i<EquipEntries.size(); ++i)
	{
		if (!ProstheticMode)
		{
			bool isClicked = ImGui::Selectable(EquipEntries[i].equipName.c_str(), false, ImGuiSelectableFlags_SetNavIdOnHover);
			if (i + 1 < EquipEntries.size())
				ImGui::Separator();

			if (!isClicked)
				continue;

			CAFunctions::CombatArts[WorkingIndex] = EquipEntries[i].menuID;
			WidgetMenu::UpdateCombatArtNames();
			IsSelectionInitialized = false;
			IsMenuOpen = false;
		}
		else
		{
			if (ImGui::Checkbox(EquipEntries[i].equipName.c_str(), selectedProsthetics + i))
			{
				if (selectedProsthetics[i])
				{
					++selectedNum;
					if (selectedNum > 3)
					{
						selectedNum = 3;
						selectedProsthetics[i] = false;
					}
					else
					{
						if (selectedNum == 1)
							ProstheticFunctions::ProstheticSets[WorkingIndex].slotOne = EquipEntries[i].menuID;
						else if (selectedNum == 2)
							ProstheticFunctions::ProstheticSets[WorkingIndex].slotTwo = EquipEntries[i].menuID;
						else if (selectedNum == 3)
							ProstheticFunctions::ProstheticSets[WorkingIndex].slotThree = EquipEntries[i].menuID;
					}
				}
				else
				{
					--selectedNum;
				}
			}

			if (i + 1 < EquipEntries.size())
				ImGui::Separator();
		}
	}

	ImGui::EndPopup();
	ImGui::End();

	return true;
}