#pragma once

#include <functional>
#include <imgui.h>

namespace Input
{
    struct ImGuiKeySet
    {
        ImGuiKey key1;
        ImGuiKey key2;
    };

    struct GameKey
    {
        bool isPressed = false;
        ImGuiKeySet KeyParams = { ImGuiKey_None, ImGuiKey_None };
        ImGuiKey ModKeyParam = ImGuiKey_None;

        void* PressArgs = nullptr;
        void* AltPressArgs = nullptr;
        void* ReleaseArgs = nullptr;
        void* AltReleaseArgs = nullptr;
        std::function<void(void*)> PerformAction = nullptr;
        std::function<void(void*)> PerformAltAction = nullptr;
        std::function<void(void*)> PerformReleaseAction = nullptr;
        std::function<void(void*)> PerformAltReleaseAction = nullptr;

        GameKey(ImGuiKeySet keyParams, std::function<void(void*)> performAction) : KeyParams(keyParams), PerformAction(performAction) {}
        GameKey(ImGuiKeySet keyParams, std::function<void(void*)> performAction, std::function<void(void*)> releaseAction) : KeyParams(keyParams), PerformAction(performAction), PerformReleaseAction(releaseAction) {}
        GameKey(ImGuiKeySet keyParams, ImGuiKey modKeyParam, std::function<void(void*)> performAction, std::function<void(void*)> performAltAction) : KeyParams(keyParams), ModKeyParam(modKeyParam), PerformAction(performAction), PerformAltAction(performAltAction) {}
        GameKey(ImGuiKeySet keyParams, ImGuiKey modKeyParam, std::function<void(void*)> performAction, std::function<void(void*)> releaseAction, std::function<void(void*)> performAltAction, std::function<void(void*)> altReleaseAction) : KeyParams(keyParams), ModKeyParam(modKeyParam), PerformAction(performAction), PerformAltAction(performAltAction), PerformReleaseAction(releaseAction), PerformAltReleaseAction(altReleaseAction) {}
        ~GameKey() { delete PressArgs; delete AltPressArgs; delete ReleaseArgs; delete AltReleaseArgs; }
    };
}