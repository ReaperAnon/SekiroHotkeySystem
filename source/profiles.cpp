#include <profiles.h>

bool IsLoaded = false;


uint64_t Profiles::DeleteSaveHook(uint64_t arg1, uint64_t arg2, unsigned saveSlot)
{
    DeleteProfile(saveSlot);
    return Hooks::DeleteSaveOrig(arg1, arg2, saveSlot);
}

unsigned Profiles::GetProfileIndex(std::filesystem::path profilePath)
{
    std::string fileName = profilePath.filename().string();
    std::string profileIdx = fileName.substr(fileName.find_first_of("0123456789"));

    return std::stoi(profileIdx) - 1;
}

void Profiles::InitProfileManager()
{
    IsLoaded = true;
    std::thread(ManageProfiles).detach();
}

void Profiles::UninitProfileManager()
{
    IsLoaded = false;
}

void Profiles::ManageProfiles()
{
    while (IsLoaded)
    {
        Sleep(500);
        static bool isProfileLoaded = false;
        uintptr_t skillBase = Hooks::GetSkillBase();
        uintptr_t characterBase = Hooks::GetCharacterBase();

        if (!isProfileLoaded && characterBase != 0) // game is loading up
        {
            LoadProfile();
            isProfileLoaded = true;
        }
        else if (isProfileLoaded && characterBase == 0) // exited to the main menu
        {
            SaveProfile();
            CAFunctions::ClearEquipmentSlots();
            ProstheticFunctions::ClearEquipmentSlots();
            isProfileLoaded = false;
        }
    }
}

void Profiles::DeleteProfile(unsigned slotIdx)
{
    std::vector<std::filesystem::path> paths;
    for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path() / "profiles"))
        paths.push_back(entry.path());

    for (const auto& entry : paths)
    {
        if (GetProfileIndex(entry) == slotIdx)
        {
            std::filesystem::remove(entry);
            break;
        }
    }

    std::filesystem::path profilesDir = std::filesystem::current_path() / "profiles";
    for (const auto& entry : paths)
    {
        unsigned profileIdx = GetProfileIndex(entry);
        if (profileIdx > slotIdx)
        {
            std::string newName = "profile";
            newName += std::to_string(profileIdx);
            newName += ".cfg";

            std::filesystem::rename(entry, profilesDir / newName);
        }
    }
}

void Profiles::SaveProfile()
{
    std::string fileName = "profile"; fileName += std::to_string(Hooks::GetSaveSlotIdx() + 1); fileName += ".cfg";
    std::filesystem::path dir = std::filesystem::current_path() / "profiles" / fileName;
    std::ofstream saveFile(dir.string());

    for (const auto& ca : CAFunctions::CombatArts)
        saveFile << ca << " ";

    saveFile << "\n";
    for (const auto& prosthetic : ProstheticFunctions::ProstheticSets)
        saveFile << prosthetic.slotOne << " " << prosthetic.slotTwo << " " << prosthetic.slotThree << " \n";

    saveFile.close();
}

void Profiles::LoadProfile()
{
    std::vector<std::filesystem::path> paths;
    for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path() / "profiles"))
        paths.push_back(entry.path().string());

    if (paths.empty())
        return;

    for (const auto& entry : paths)
    {
        if (GetProfileIndex(entry) != Hooks::GetSaveSlotIdx())
            continue;

        char* end;
        char buffer[100];
        std::ifstream profile(entry);

        profile.get(buffer, 100, '\n'); profile.ignore();
        std::string caBuffer(buffer);

        size_t lastIdx = 0;
        size_t prevLastIdx = 0;
        for (int i = 0; i < CAFunctions::CombatArts.size(); i++)
        {
            lastIdx = caBuffer.find_first_of(' ', lastIdx + 1);
            if (lastIdx == std::string::npos)
                break;

            CAFunctions::CombatArts[i] = (unsigned)std::stoi(caBuffer.substr(prevLastIdx, lastIdx));
            prevLastIdx = lastIdx;
        }

        for (int i = 0; i < ProstheticFunctions::ProstheticSets.size(); i++)
        {
            profile.get(buffer, 100, ' '); profile.ignore();
            if (profile.eof())
                break;

            ProstheticFunctions::ProstheticSets[i].slotOne = (unsigned)std::strtoul(buffer, &end, 10);
            profile.get(buffer, 100, ' '); profile.ignore();
            ProstheticFunctions::ProstheticSets[i].slotTwo = (unsigned)std::strtoul(buffer, &end, 10);
            profile.get(buffer, 100, ' '); profile.ignore();
            ProstheticFunctions::ProstheticSets[i].slotThree = (unsigned)std::strtoul(buffer, &end, 10);
            profile.ignore();
        }

        profile.close();

        std::vector<unsigned> combatArts = Hooks::GetCombatArts();
        std::vector<unsigned> prostheticTools = Hooks::GetProstheticTools();
        for (const auto& art : CAFunctions::CombatArts)
        {
            if (std::find(combatArts.begin(), combatArts.end(), art) == combatArts.end())
            {
                CAFunctions::ClearEquipmentSlots();
                break;
            }
        }

        for (const auto& prosthetic : ProstheticFunctions::ProstheticSets)
        {
            
            if ((std::find(prostheticTools.begin(), prostheticTools.end(), prosthetic.slotOne) == prostheticTools.end() ||
                 std::find(prostheticTools.begin(), prostheticTools.end(), prosthetic.slotTwo) == prostheticTools.end() ||
                 std::find(prostheticTools.begin(), prostheticTools.end(), prosthetic.slotThree) == prostheticTools.end()) &&
                 prosthetic.slotOne != PROSTHETIC_EMPTY_SLOT &&
                 prosthetic.slotTwo != PROSTHETIC_EMPTY_SLOT &&
                 prosthetic.slotThree != PROSTHETIC_EMPTY_SLOT
                )
            {
                ProstheticFunctions::ClearEquipmentSlots();
                break;
            }
        }

        return;
    }
}
