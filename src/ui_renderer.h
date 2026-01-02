#pragma once

#include "types.h"
#include <windows.h>

class UIRenderer {
public:
    static void Render(HWND hwnd, const AppState& state);
    
private:
    static void RenderServerList(HDC hdc, const RECT& rect);
    static void RenderChannelList(HDC hdc, const RECT& rect, const AppState& state);
    static void RenderChatArea(HDC hdc, const RECT& rect, const AppState& state);
    static void RenderInputBar(HDC hdc, const RECT& rect);
    static void RenderUserBar(HDC hdc, const RECT& rect);
    
    // Cores oficiais do Discord (Dark Theme)
    static constexpr COLORREF COLOR_SERVER_LIST = RGB(30, 33, 36);      // #1e2124
    static constexpr COLORREF COLOR_CHANNEL_LIST = RGB(43, 45, 49);    // #2b2d31
    static constexpr COLORREF COLOR_MAIN_BG = RGB(49, 51, 56);          // #313338
    static constexpr COLORREF COLOR_INPUT_BAR = RGB(64, 68, 75);        // #40444b
    static constexpr COLORREF COLOR_USER_BAR = RGB(43, 45, 49);         // #2b2d31
    static constexpr COLORREF COLOR_DISCORD_BLUE = RGB(88, 101, 242);   // #5865f2
    static constexpr COLORREF COLOR_TEXT_PRIMARY = RGB(219, 222, 225);  // #dbdee1
    static constexpr COLORREF COLOR_TEXT_SECONDARY = RGB(181, 186, 193); // #b5bac1
    static constexpr COLORREF COLOR_TEXT_MUTED = RGB(142, 146, 151);    // #8e9297
    static constexpr COLORREF COLOR_CHANNEL_HOVER = RGB(57, 60, 67);   // #393c43
    static constexpr COLORREF COLOR_SEPARATOR = RGB(78, 80, 88);        // #4e5058
};
