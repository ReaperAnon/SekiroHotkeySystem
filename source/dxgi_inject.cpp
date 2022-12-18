#include <dxgi_inject.h>

bool IsInitialized = false;
ID3D11Device* D3D11Device = nullptr;
ID3D11DeviceContext* D3D11DeviceContext = nullptr;
ID3D11RenderTargetView* D3D11RenderTargetView = nullptr;

AOBScanner patternScanner(L"dxgi.dll");
const short vTablePattern[] = { 0x90, 0x48, 0x8D, 0x05, 0x100, 0x100, 0x100, 0x100, 0x48, 0x89, 0x07 };
const short vTableOffset = 0x01;

typedef DWORD(WINAPI* tPresent)(IDXGISwapChain* swapChain, unsigned syncInterval, unsigned flags);
    tPresent Present = nullptr;
    tPresent PresentOriginal = nullptr;

const bool DXGI::HookDXGI()
{
    WNDCLASSEX windowClass;
    ZeroMemory(&windowClass, sizeof(windowClass));
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.lpszClassName = L"TempWndClass";
    windowClass.hInstance = GetModuleHandle(NULL);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = DefWindowProc;
    RegisterClassEx(&windowClass);

    HWND tempWindow = CreateWindow(windowClass.lpszClassName, L"TempDXWindow", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.OutputWindow = tempWindow;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };

    bool result = false;
    IDXGISwapChain* swapChainTemp = nullptr;
    ID3D11Device* D3D11DeviceTemp = nullptr;
    if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &swapChainTemp, &D3D11DeviceTemp, nullptr, nullptr)))
    {
        void** vTable = *reinterpret_cast<void***>(swapChainTemp);
        Present = (tPresent)vTable[8];
        swapChainTemp->Release();
        D3D11DeviceTemp->Release();

#ifdef HOTKEYS_DEBUG
        std::cout << "DXGI vTable Address: " << vTable;
#endif

        result = MH_CreateHook(Present, PresentDetour, (LPVOID*)&PresentOriginal) == MH_OK && MH_EnableHook(Present) == MH_OK;
    }

    DestroyWindow(tempWindow);
    UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

#ifdef HOTKEYS_DEBUG
    result ? std::cout << "\nDXGI Hooked\n" : std::cout << "\nDXGI Hook Failed\n";
#endif

    return result;
}

/* FAILS IF RESHADE OR SIMILAR ARE ALSO LOADED
const bool DXGI::HookDXGI()
{
    bool result = false;
    BYTE* vTableAddr = patternScanner.PerformModuleScan(vTablePattern, sizeof(vTablePattern) / sizeof(vTablePattern[0]), vTableOffset);
    if (vTableAddr)
    {
        long ptrOffset = *reinterpret_cast<long*>(vTableAddr + 3);
        void** vTable = reinterpret_cast<void**>(vTableAddr + ptrOffset + 7);

        Present = (tPresent)vTable[8];
#ifdef HOTKEYS_DEBUG
        std::cout << "DXGI vTable Address: " << vTable;
#endif

        result = MH_CreateHook(Present, PresentDetour, (LPVOID*)&PresentOriginal) == MH_OK && MH_EnableHook(Present) == MH_OK;
    }

#ifdef HOTKEYS_DEBUG
    result ? std::cout << "\nDXGI Hooked\n" : std::cout << "\nDXGI Hook Failed\n";
#endif

    return result;
}
*/

const void DXGI::UnhookDXGI()
{
    MH_DisableHook(Present);
    ImGui::UninitImGui();
    D3D11RenderTargetView->Release();
    D3D11RenderTargetView = nullptr;
    D3D11DeviceContext->Release();
    D3D11DeviceContext = nullptr;
    D3D11Device->Release();
    D3D11Device = nullptr;
}


LONG WINAPI DXGI::PresentDetour(IDXGISwapChain* swapChain, unsigned syncInterval, unsigned flags)
{
    if (!IsInitialized)
    {
        if (SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D11Device), (LPVOID*)&D3D11Device)))
        {
            // Get the device context
            D3D11Device->GetImmediateContext(&D3D11DeviceContext);

            // Initialize the back buffer and create the render target where ImGui will be rendered to
            ID3D11Texture2D* backBuffer = nullptr;
            swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
            if (backBuffer != nullptr)
            {
                D3D11Device->CreateRenderTargetView(backBuffer, NULL, &D3D11RenderTargetView);
                backBuffer->Release();

                // Initialize ImGui
                ImGui::InitImGuiDX11(D3D11Device, D3D11DeviceContext);

                IsInitialized = true;
            }
        }

        return PresentOriginal(swapChain, syncInterval, flags);
    }

    // Draw the menus on-screen.
    ImGui::ImGuiPresent(D3D11DeviceContext, D3D11RenderTargetView);

    return PresentOriginal(swapChain, syncInterval, flags);
}
