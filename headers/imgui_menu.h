#pragma once

#include <imgui.h>

namespace ImGui
{
    struct WidgetSettings
    {
        bool showHotkeys = 1;
        float widgetScale = 1;

        int widgetMode = 0;
        int widgetPosition = 0;
        int widgetColor = 0;
    };

    float GetResolutionScale();

    ImVec2 GetScaledScreenCoord(ImVec2 coords);
}
