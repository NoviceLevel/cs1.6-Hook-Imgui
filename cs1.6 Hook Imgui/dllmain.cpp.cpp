#include <windows.h>
#include <gl/GL.h>
#include <stdint.h>
#include "MinHook.h"
#include "gui.h"
#include "imgui.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include <cstdio>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "libMinHook.x86.lib")


typedef BOOL(WINAPI* wglSwapBuffers_t)(HDC);
wglSwapBuffers_t o_wglSwapBuffers = nullptr;

WNDPROC g_origWndProc = nullptr;
HWND g_hwnd = nullptr;
bool g_inited = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;
    return CallWindowProc(g_origWndProc, hwnd, uMsg, wParam, lParam);
}


BOOL WINAPI hk_wglSwapBuffers(HDC hdc) {
    if (!g_inited) {
        g_hwnd = WindowFromDC(hdc);

        // 初始化 ImGui
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        ImGui_ImplWin32_Init(g_hwnd);
        ImGui_ImplOpenGL3_Init("#version 130");

        g_origWndProc = (WNDPROC)SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

        g_inited = true;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    handle_menu_toggle(); // 处理菜单显示/隐藏和鼠标指针
    Misaki_Menu();       // 菜单绘制


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return o_wglSwapBuffers(hdc);
}


// 控制台输出
void setup_console() {
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 彩色输出标题
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("  __  __ _ _       _        _ _ \n");
    printf(" |  \\/  (_) | __ _| | _____| | |\n");
    printf(" | |\\/| | | |/ _` | |/ / _ \\ | |\n");
    printf(" | |  | | | | (_| |   <  __/ | |\n");
    printf(" |_|  |_|_|_|\\__,_|_|\\_\\___|_|_|\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("       OpenGL Hook LinkStart \n");
    printf("================================\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("[*] Press DELETE to toggle menu\n");
    printf("[*] Enjoy Misaki Menu!\n");
    printf("================================\n");
}

DWORD WINAPI init_hooks(LPVOID lpParameter) {
    setup_console();

    // Hook wglSwapBuffers
    HMODULE hOpenGL = GetModuleHandleW(L"opengl32.dll");
    if (hOpenGL) {
        void* target = GetProcAddress(hOpenGL, "wglSwapBuffers");
        if (target) {
            MH_Initialize();
            MH_CreateHook(target, hk_wglSwapBuffers, reinterpret_cast<void**>(&o_wglSwapBuffers));
            MH_EnableHook(target);
        }
    }

    return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(NULL, 0, init_hooks, NULL, 0, NULL);
    }
    return TRUE;
}

int main() { return 0; }
