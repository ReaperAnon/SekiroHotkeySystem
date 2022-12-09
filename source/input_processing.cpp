#include <input_processing.h>

std::vector<Input::ImGuiKeySet> Input::CombatArtKeys;

std::vector<Input::ImGuiKeySet> Input::ProstheticSetKeys;

Input::ImGuiKeySet Input::ProstheticKeys[3];

Input::ImGuiKeySet Input::CombatArtKey;

std::vector<Input::GameKey*> Input::GameKeys;

HWND Input::PROCHWND;

bool InputBlocks[ImGuiKey_NamedKey_COUNT];

bool IsInputLoaded = false;


void Input::InitInputProcessing()
{
    IsInputLoaded = true;
    EnumWindows(WorkerProc, 0);
    std::cout << "Sekiro HWND: " << std::hex << PROCHWND << "\n";
    ImGui::InitImGuiWin32();
    _beginthread(InputProcessThread, NULL, nullptr);
}

void Input::UninitInputProcessing()
{
    IsInputLoaded = false;
}

void Input::InputProcessThread(void* args)
{
    while (IsInputLoaded)
    {
        Sleep(50);
        ProcessKeyEvents();
    }
}

void Input::ProcessKeyEvents()
{
    if (GetForegroundWindow() != Input::PROCHWND) return;

    for (auto gameKey : GameKeys)
    {
        bool isKeyPressed = (gameKey->KeyParams.key1 != ImGuiKey_None && gameKey->KeyParams.key2 == ImGuiKey_None) ? ImGui::IsKeyDown(gameKey->KeyParams.key1) : (gameKey->KeyParams.key1 != ImGuiKey_None && gameKey->KeyParams.key2 != ImGuiKey_None) ? (ImGui::IsKeyDown(gameKey->KeyParams.key1) && ImGui::IsKeyDown(gameKey->KeyParams.key2)) : false;
        if (isKeyPressed)
        {
            if (InputBlocks[gameKey->KeyParams.key1 - 512] || (gameKey->KeyParams.key2 != ImGuiKey_None && InputBlocks[gameKey->KeyParams.key2 - 512]))
                continue;

            InputBlocks[gameKey->KeyParams.key1 - 512] = 1;
            if(gameKey->KeyParams.key2 != ImGuiKey_None)
                InputBlocks[gameKey->KeyParams.key2 - 512] = 1;

            if (gameKey->isPressed)
                continue;

            gameKey->isPressed = true;
            if (gameKey->PerformAltAction && ImGui::IsKeyDown(gameKey->ModKeyParam))
                gameKey->PerformAltAction(gameKey->AltPressArgs);
            else
                gameKey->PerformAction(gameKey->PressArgs);
        }
        else if (gameKey->isPressed)
        {
            gameKey->isPressed = false;
            InputBlocks[gameKey->KeyParams.key1 - 512] = 0;
            if (gameKey->KeyParams.key2 != ImGuiKey_None)
                InputBlocks[gameKey->KeyParams.key2 - 512] = 0;

            if (gameKey->PerformAltReleaseAction && ImGui::IsKeyDown(gameKey->ModKeyParam))
                gameKey->PerformAltReleaseAction(gameKey->AltReleaseArgs);
            else if (gameKey->PerformReleaseAction)
                gameKey->PerformReleaseAction(gameKey->ReleaseArgs);
        }
    }
}

BOOL CALLBACK Input::WorkerProc(HWND hwnd, LPARAM lParam)
{
    static TCHAR buffer[50];
    GetWindowText(hwnd, buffer, 50);
    if (wcsstr(buffer, L"Sekiro"))
    {
        Input::PROCHWND = hwnd;
        return FALSE;
    }

    return TRUE;
}

LRESULT WINAPI Input::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(Input::OLDWNDPROC, hWnd, uMsg, wParam, lParam);
}

// Takes regular Direct Input key params. First number in the array of keys to press is the size of the array - 1.
void Input::PressKey(void* keysToPress)
{
    int length = *reinterpret_cast<int*>(keysToPress);
    for (int i = 1; i <= length; i++)
    {
        int keyParam = reinterpret_cast<int*>(keysToPress)[i];
        if (keyParam == 0x1)
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        else if (keyParam == 0x2)
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        else
            keybd_event(keyParam, MapVirtualKey(keyParam, 0), 0, 0);
    }
}

// Takes regular Direct Input key params. First number in the array of keys to press is the size of the array - 1.
void Input::ReleaseKey(void* keysToRelease)
{
    int length = *reinterpret_cast<int*>(keysToRelease);
    for (int i = length; i > 0; i--)
    {
        int keyParam = reinterpret_cast<int*>(keysToRelease)[i];
        if (keyParam == 0x1)
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        else if (keyParam == 0x2)
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        else
            keybd_event(keyParam, MapVirtualKey(keyParam, 0), KEYEVENTF_KEYUP, 0);
    }
}