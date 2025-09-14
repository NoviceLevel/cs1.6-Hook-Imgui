#include "imgui.h"
#include "gui.h"

namespace relicgogo {
    namespace visuals {
        inline bool box = false;

    }
}

static bool g_show_Misaki_Menu = true;
static float g_Misaki_Menu_alpha = 0.0f;

 
void handle_menu_toggle() {
    if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
        g_show_Misaki_Menu = !g_show_Misaki_Menu;
        ImGui::GetIO().MouseDrawCursor = g_show_Misaki_Menu;
    }
}

void Misaki_Menu() {
 
    if (g_show_Misaki_Menu && g_Misaki_Menu_alpha < 1.0f) g_Misaki_Menu_alpha += 0.08f;
    if (!g_show_Misaki_Menu && g_Misaki_Menu_alpha > 0.0f) g_Misaki_Menu_alpha -= 0.08f;
    if (g_Misaki_Menu_alpha <= 0.01f) return;
 
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.12f, 0.18f, 0.95f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.20f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.16f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.30f, 0.80f, 0.40f, 1.00f);

    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Always); // 固定宽高
    ImGui::SetNextWindowBgAlpha(g_Misaki_Menu_alpha);             // 渐显渐隐
    ImGui::Begin("Misaki", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize); // 不可缩放
    {
        ImGui::Checkbox(reinterpret_cast<const char*>(u8"ESP"), &relicgogo::visuals::box);
        ImGui::Separator();

    }
    ImGui::End();
}
