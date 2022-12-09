#include <aobscanner.h>

BYTE* AOBScanner::ScanPattern(const SHORT* pattern, uintptr_t patternLen, BYTE* begin, uintptr_t size)
{
    uintptr_t patternIdx = 0;

    for (uintptr_t i = 0; i < size; ++i)
    {
        if (pattern[patternIdx] > 0xFF || pattern[patternIdx] == begin[i])
            ++patternIdx;
        else
            patternIdx = 0;

        if (patternIdx == patternLen)
            return begin + i - patternLen + 1;
    }

    return nullptr;
}

BYTE* AOBScanner::ScanModuleInternal(const SHORT* pattern, uintptr_t patternLen, BYTE* begin, uintptr_t size)
{
    BYTE* resultAddress = nullptr;
    MEMORY_BASIC_INFORMATION    mbi{};

    for (BYTE* currByte = begin; currByte < begin + size; currByte += mbi.RegionSize)
    {
        if (!VirtualQuery(currByte, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
            continue;

        resultAddress = ScanPattern(pattern, patternLen, currByte, mbi.RegionSize);
        if (resultAddress != nullptr)
            break;
    }

    return resultAddress;
}

AOBScanner::Module AOBScanner::GetModuleData(const WCHAR* moduleName)
{
    HANDLE                  hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(GetCurrentProcess()));
    Module                  moduleData{ nullptr, 0 };
    MODULEENTRY32           moduleEntry;
    moduleEntry.dwSize = sizeof(moduleEntry);
    
    Module32First(hModule, &moduleEntry);
    do {
        if (!wcscmp(moduleEntry.szModule, moduleName))
        {
            CloseHandle(hModule);
            moduleData.moduleBase = moduleEntry.modBaseAddr;
            moduleData.moduleSize = moduleEntry.modBaseSize;
            break;
        }
    } while (Module32Next(hModule, &moduleEntry));

    return moduleData;
}

BYTE *AOBScanner::GetStaticAddress(uintptr_t offset)
{
    return (BYTE *)moduleData.moduleBase + offset;
}

BYTE* AOBScanner::ResolvePointerChain(BYTE* baseAddress, std::vector<int> offsets)
{
    BYTE* retAddress = baseAddress;
    for(auto offset : offsets)
    {
        ReadProcessMemory(GetCurrentProcess(), retAddress, &retAddress, sizeof(retAddress), 0);
        if (retAddress == nullptr)
            break;
        retAddress += offset;
    }

    return ((uintptr_t)retAddress) < ((uintptr_t)moduleData.moduleBase) ? nullptr : retAddress;
}

BYTE* AOBScanner::PerformModuleScan(const SHORT* pattern, uintptr_t patternLen, uintptr_t patternOffset)
{
    BYTE* result = ScanModuleInternal(pattern, patternLen, (BYTE*)moduleData.moduleBase, moduleData.moduleSize);

    return result ? result + patternOffset : nullptr;
}

BYTE* AOBScanner::PerformRegionScan(const SHORT* pattern, uintptr_t patternLen, uintptr_t patternOffset, uintptr_t offset, uintptr_t size)
{
    BYTE* result = ScanModuleInternal(pattern, patternLen, (BYTE*)moduleData.moduleBase + offset, size) + patternOffset;

    return result ? result + patternOffset : nullptr;
}