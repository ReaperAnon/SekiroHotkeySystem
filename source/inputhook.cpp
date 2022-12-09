#include "framework.h"

namespace InputHook
{
    // Settings


    // D3D11 Hook

    HWND procHWND = nullptr;
    WNDPROC oldWndProc;
    const wchar_t procName[] = L"Sekiro";


    // Console
    FILE* console = nullptr;





    bool IsConfigOpen()
    {
        return configOpen || ImGui::GetIO().WantCaptureMouse;
    }



    void InitializeHook()
    {
        EnumWindows(WorkerProc, 0);

        // AllocConsole();
        // freopen_s(&console, "CONOUT$", "w", stdout);

        HookDXGI() ? std::cout << "DXGI Hooked\n" : std::cout << "DXGI Hook Failed\n";
        
        std::cout << "Input Hook Loaded\nSekiro HWND: " << procHWND << "\n";

        ReadConfigFile();
        ReloadSettings();
    }

    void RemoveHook()
    {
        UnhookDXGI();
        if (console)
        {
            fclose(console);
            FreeConsole();
        }
    }

}