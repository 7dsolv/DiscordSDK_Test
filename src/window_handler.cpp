#include "window_handler.h"
#include "ui_renderer.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

WindowHandler::WindowHandler() 
    : hwnd_(nullptr)
    , hInstance_(nullptr)
    , discordClient_(nullptr)
{
}

WindowHandler::~WindowHandler() {
    Shutdown();
}

bool WindowHandler::Initialize(HINSTANCE hInstance, int nCmdShow, DiscordClient* discordClient) {
    hInstance_ = hInstance;
    discordClient_ = discordClient;
    
    const char CLASS_NAME[] = "DiscordSDKTestWindow";
    
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    
    // Armazenar ponteiro para a instância
    wc.cbWndExtra = sizeof(WindowHandler*);
    
    RegisterClassA(&wc);
    
    // Criar janela (tamanho otimizado para layout Discord)
    hwnd_ = CreateWindowExA(
        0,
        CLASS_NAME,
        "Discord SDK Test - Rich Presence Integration",
        WS_OVERLAPPEDWINDOW,
        50, 50, 1280, 720,
        nullptr,
        nullptr,
        hInstance,
        this  // Passar this como lParam
    );
    
    if (hwnd_ == nullptr) {
        return false;
    }
    
    // Armazenar ponteiro na janela
    SetWindowLongPtrA(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    
    // Adicionar mensagens iniciais
    AddChatMessage("System", "Discord SDK inicializado com sucesso!", true);
    AddChatMessage("Bot", "Pressione C para Chat, V para Voice, R para Recording", true);
    if (discordClient_ && discordClient_->IsConnected()) {
        AddChatMessage("Discord", "Rich Presence ativo! Verifique seu perfil no Discord.", true);
    }
    
    ShowWindow(hwnd_, nCmdShow);
    UpdateWindow(hwnd_);
    
    return true;
}

void WindowHandler::Run() {
    MSG msg = {};
    while (GetMessageA(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
        
        // Executar callbacks do Discord periodicamente
        if (discordClient_) {
            discordClient_->RunCallbacks();
        }
    }
}

void WindowHandler::Shutdown() {
    if (hwnd_) {
        DestroyWindow(hwnd_);
        hwnd_ = nullptr;
    }
}

LRESULT CALLBACK WindowHandler::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WindowHandler* handler = GetInstance(hwnd);
    
    switch (uMsg) {
    case WM_CREATE:
        {
            CREATESTRUCTA* cs = reinterpret_cast<CREATESTRUCTA*>(lParam);
            if (cs && cs->lpCreateParams) {
                handler = reinterpret_cast<WindowHandler*>(cs->lpCreateParams);
                SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(handler));
            }
        }
        return 0;
        
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        
    case WM_COMMAND:
        if (handler) {
            handler->HandleCommand(LOWORD(wParam));
        }
        return 0;
        
    case WM_KEYDOWN:
        if (handler) {
            handler->HandleKeyDown(wParam);
        }
        return 0;
        
    case WM_PAINT:
        if (handler) {
            UIRenderer::Render(hwnd, handler->state_);
        }
        return 0;
        
    default:
        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
}

WindowHandler* WindowHandler::GetInstance(HWND hwnd) {
    LONG_PTR ptr = GetWindowLongPtrA(hwnd, GWLP_USERDATA);
    return reinterpret_cast<WindowHandler*>(ptr);
}

void WindowHandler::HandleCommand(int commandId) {
    switch (commandId) {
    case 1001: // Chat
        {
            int msgNum = static_cast<int>(state_.chatHistory.size()) + 1;
            AddChatMessage("User" + std::to_string(msgNum), 
                          "Hello from Discord SDK! Message #" + std::to_string(msgNum));
            
            // Atualizar Rich Presence
            if (discordClient_ && discordClient_->IsConnected()) {
                discordClient_->UpdateRichPresence(
                    "No canal #general",
                    "Testando Discord SDK",
                    "discord_logo",
                    "Discord SDK Test"
                );
            }
        }
        break;
        
    case 1002: // Voice Toggle
        state_.isVoiceActive = !state_.isVoiceActive;
        if (state_.isVoiceActive) {
            AddChatMessage("System", "Conectado ao canal de voz", true);
            if (discordClient_ && discordClient_->IsConnected()) {
                discordClient_->UpdateRichPresence(
                    "Em canal de voz",
                    "The Crew - Voice Channel",
                    "voice",
                    "Canal de Voz"
                );
            }
        } else {
            AddChatMessage("System", "Desconectado do canal de voz", true);
        }
        InvalidateRect(hwnd_, nullptr, FALSE);
        break;
        
    case 1003: // Recording Toggle
        state_.isRecording = !state_.isRecording;
        if (state_.isRecording) {
            AddChatMessage("System", "Gravação iniciada", true);
        } else {
            AddChatMessage("System", "Gravação parada", true);
        }
        InvalidateRect(hwnd_, nullptr, FALSE);
        break;
    }
    
    InvalidateRect(hwnd_, nullptr, FALSE);
}

void WindowHandler::HandleKeyDown(WPARAM key) {
    if (key == 'C' || key == 'c') {
        SendMessage(hwnd_, WM_COMMAND, 1001, 0);
    }
    else if (key == 'V' || key == 'v') {
        SendMessage(hwnd_, WM_COMMAND, 1002, 0);
    }
    else if (key == 'R' || key == 'r') {
        SendMessage(hwnd_, WM_COMMAND, 1003, 0);
    }
    else if (key == VK_ESCAPE) {
        PostMessage(hwnd_, WM_DESTROY, 0, 0);
    }
}

void WindowHandler::AddChatMessage(const std::string& username, const std::string& message, bool isBot) {
    ChatMessage msg;
    msg.userId = "user_" + std::to_string(rand() % 9999);
    msg.username = username;
    msg.message = message;
    msg.timestamp = GetCurrentTime();
    msg.isBot = isBot;
    
    state_.chatHistory.push_back(msg);
    
    // Limitar histórico a 50 mensagens
    if (state_.chatHistory.size() > 50) {
        state_.chatHistory.erase(state_.chatHistory.begin());
    }
}

std::string WindowHandler::GetCurrentTime() {
    auto now = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &now);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M");
    return oss.str();
}
