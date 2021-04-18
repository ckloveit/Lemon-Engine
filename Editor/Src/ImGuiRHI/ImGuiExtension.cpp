#include "ImGuiExtension.h"

namespace ImGuiEx
{
    ImVec4 SelectedColour = ImVec4(0.28f, 0.56f, 0.9f, 1.0f);
    void Tooltip(const std::string& text)
    {
        Tooltip(text.c_str());
    }
	
    void Tooltip(const char* text)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted(text);
            ImGui::EndTooltip();
        }
        ImGui::PopStyleVar();
    }
    
    ImVec4 GetSelectedColor() {return SelectedColour;}
}