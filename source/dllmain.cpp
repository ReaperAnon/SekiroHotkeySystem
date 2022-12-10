#include "dllmain.h"

// The base address of DInput8.dll
HMODULE DInput8DLL = nullptr;

// The function pointer to the original DirectInput8Create.
tDirectInput8Create DirectInput8CreateOrig = nullptr;

// Name of the dll to chain load instead of the original dinput8 one.
WCHAR dllPath[MAX_PATH] = L"";

FILE* console = nullptr;


void HookConsole()
{
    AllocConsole();
    freopen_s(&console, "CONOUT$", "w", stdout);
    SetConsoleTitle(L"Debug Console");
}

void UnhookConsole()
{
    if (console)
    {
        fclose(console);
        FreeConsole();
    }
}

void HookThread(void* args)
{
    LoadDll();
    MH_Initialize();

    Sleep(10000);

    // HookConsole();
    Input::InitInputProcessing();
    DXGI::HookDXGI();
    Hooks::HookFunctions();
    Profiles::InitProfileManager();
    Configs::ReadConfigFile();
}

void LoadDll()
{
    GetPrivateProfileString(L"Misc", L"chainDllName", L"", dllPath, 100, L".\\configs\\chainload.ini");
    if (wcslen(dllPath) > 0)
    {
        DInput8DLL = LoadLibrary(dllPath);
        DirectInput8CreateOrig = (tDirectInput8Create)GetProcAddress(DInput8DLL, "DirectInput8Create");
    }

    if (DirectInput8CreateOrig == nullptr)
    {
        GetSystemDirectory(dllPath, MAX_PATH);
        wcscat_s(dllPath, L"\\dinput8.dll");
        DInput8DLL = LoadLibrary(dllPath);
        DirectInput8CreateOrig = (tDirectInput8Create)GetProcAddress(DInput8DLL, "DirectInput8Create");
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD callReason, LPVOID lpReserved)
{
    switch (callReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            _beginthread(HookThread, 0, nullptr);
        break;

        case DLL_PROCESS_DETACH:
            Profiles::UninitProfileManager();
            Input::UninitInputProcessing();
            DXGI::UnhookDXGI();
            UnhookConsole();

            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
        break;
    }

    return TRUE;
}

extern "C"
{
    FUNCWRAPPER DirectInput8Create(HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
    {
        return DirectInput8CreateOrig(hInst, dwVersion, riidltf, ppvOut, punkOuter);
    }
}
