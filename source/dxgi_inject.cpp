#include <dxgi_inject.h>

bool IsInitialized;
ID3D11Device* D3D11Device;
ID3D11DeviceContext* D3D11DeviceContext;
ID3D11RenderTargetView* D3D11RenderTargetView;

AOBScanner patternScanner(L"dxgi.dll");
const short vTablePattern[] = { 0x90, 0x48, 0x8D, 0x05, 0x100, 0x100, 0x100, 0x100, 0x48, 0x89, 0x07 };
const short vTableOffset = 0x01;

typedef DWORD(WINAPI* tPresent)(IDXGISwapChain* swapChain, unsigned syncInterval, unsigned flags);
    tPresent Present = nullptr;
    tPresent PresentOriginal = nullptr;

/*
const bool DXGI::HookDXGI()
{
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    desc.OutputWindow = procHWND;
    desc.SampleDesc.Count = 1;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    IDXGISwapChain* swapChainTemp = nullptr;
    ID3D11Device* D3D11DeviceTemp = nullptr;

    if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &desc, &swapChainTemp, &D3D11DeviceTemp, nullptr, nullptr)))
    {
        void** vTable = *(void***)swapChainTemp;
        Present = (tPresent)vTable[8];
        swapChainTemp->Release();
        D3D11DeviceTemp->Release();

        return MH_CreateHook((LPVOID)Present, (LPVOID)PresentDetour, (LPVOID*)&PresentOriginal) == MH_OK && MH_EnableHook(Present) == MH_OK;
    }

    return false;
}
*/

const bool DXGI::HookDXGI()
{
    BYTE* vTableAddr = patternScanner.PerformModuleScan(vTablePattern, sizeof(vTablePattern) / sizeof(vTablePattern[0]), vTableOffset);
    if (vTableAddr)
    {
        long ptrOffset = *(long*)(vTableAddr + 3);
        void** vTable = (void**)(vTableAddr + ptrOffset + 7);

        Present = (tPresent)vTable[8];
        std::cout << "DXGI vTable Address: " << vTable << "\nDXGI Hooked\n";

        return MH_CreateHook(Present, PresentDetour, (LPVOID*)&PresentOriginal) == MH_OK && MH_EnableHook(Present) == MH_OK;
    }

    std::cout << "DXGI Not Hooked\n";
    return false;
}

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
            D3D11Device->CreateRenderTargetView(backBuffer, NULL, &D3D11RenderTargetView);
            backBuffer->Release();

            // Initialize ImGui
            ImGui::InitImGuiDX11(D3D11Device, D3D11DeviceContext);

            IsInitialized = true;
        }

        return PresentOriginal(swapChain, syncInterval, flags);
    }

    // Draw the menus on-screen.
    ImGui::ImGuiPresent(D3D11DeviceContext, D3D11RenderTargetView);

    return PresentOriginal(swapChain, syncInterval, flags);
}
