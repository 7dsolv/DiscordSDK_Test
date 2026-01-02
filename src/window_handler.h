#pragma once

#include "types.h"
#include "discord_client.h"
#include <windows.h>

class WindowHandler {
public:
    WindowHandler();
    ~WindowHandler();
    
    bool Initialize(HINSTANCE hInstance, int nCmdShow, DiscordClient* discordClient);
    void Run();
    void Shutdown();
    
    AppState& GetState() { return state_; }
    
private:
    HWND hwnd_;
    HINSTANCE hInstance_;
    DiscordClient* discordClient_;
    AppState state_;
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static WindowHandler* GetInstance(HWND hwnd);
    
    void HandleCommand(int commandId);
    void HandleKeyDown(WPARAM key);
    void AddChatMessage(const std::string& username, const std::string& message, bool isBot = false);
    std::string GetCurrentTime();
};
