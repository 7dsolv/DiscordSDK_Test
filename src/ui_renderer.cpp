#include "ui_renderer.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <cstring>

// Helper para desenhar texto centralizado
static void DrawTextCentered(HDC hdc, const char* text, RECT& rect, HFONT font, COLORREF color) {
    SetTextColor(hdc, color);
    HFONT hOldFont = (HFONT)SelectObject(hdc, font);
    SetTextAlign(hdc, TA_CENTER | TA_TOP);
    int len = static_cast<int>(strlen(text));
    TextOutA(hdc, (rect.left + rect.right) / 2, rect.top, text, len);
    SetTextAlign(hdc, TA_LEFT | TA_TOP);
    SelectObject(hdc, hOldFont);
}

void UIRenderer::Render(HWND hwnd, const AppState& state) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    
    // Habilitar anti-aliasing
    SetBkMode(hdc, TRANSPARENT);
    SetTextCharacterExtra(hdc, 0);
    
    RECT rect;
    GetClientRect(hwnd, &rect);
    
    // Limpar fundo com cor do Discord
    HBRUSH hBgBrush = CreateSolidBrush(COLOR_MAIN_BG);
    FillRect(hdc, &rect, hBgBrush);
    DeleteObject(hBgBrush);
    
    // Renderizar componentes com dimensões precisas do Discord
    RECT serverListRect = { 0, 0, 72, rect.bottom };
    RenderServerList(hdc, serverListRect);
    
    RECT channelListRect = { 72, 0, 240, rect.bottom - 52 };
    RenderChannelList(hdc, channelListRect, state);
    
    RECT chatAreaRect = { 240, 0, rect.right, rect.bottom - 52 };
    RenderChatArea(hdc, chatAreaRect, state);
    
    RECT inputBarRect = { 240, rect.bottom - 52, rect.right, rect.bottom };
    RenderInputBar(hdc, inputBarRect);
    
    RECT userBarRect = { 72, rect.bottom - 52, 240, rect.bottom };
    RenderUserBar(hdc, userBarRect);
    
    EndPaint(hwnd, &ps);
}

void UIRenderer::RenderServerList(HDC hdc, const RECT& rect) {
    // Fundo escuro
    HBRUSH hBrush = CreateSolidBrush(COLOR_SERVER_LIST);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    
    // Logo Discord - círculo perfeito
    int logoSize = 48;
    int logoX = 12;
    int logoY = 12;
    
    // Círculo azul do Discord
    HBRUSH hLogoBg = CreateSolidBrush(COLOR_DISCORD_BLUE);
    HPEN hLogoPen = CreatePen(PS_SOLID, 0, COLOR_DISCORD_BLUE);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hLogoPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hLogoBg);
    Ellipse(hdc, logoX, logoY, logoX + logoSize, logoY + logoSize);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hLogoBg);
    DeleteObject(hLogoPen);
    
    // Letra D branca no logo
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hLogoFont = CreateFontA(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hLogoFont);
    RECT textRect = { logoX, logoY, logoX + logoSize, logoY + logoSize };
    DrawTextCentered(hdc, "D", textRect, hLogoFont, RGB(255, 255, 255));
    SelectObject(hdc, hOldFont);
    DeleteObject(hLogoFont);
    
    // Separador fino
    HPEN hSepPen = CreatePen(PS_SOLID, 1, RGB(30, 33, 36));
    hOldPen = (HPEN)SelectObject(hdc, hSepPen);
    int sepY = logoY + logoSize + 8;
    MoveToEx(hdc, 0, sepY, nullptr);
    LineTo(hdc, rect.right, sepY);
    SelectObject(hdc, hOldPen);
    DeleteObject(hSepPen);
    
    // Ícones de servidor - círculos coloridos com hover effect
    int serverIconY = sepY + 12;
    int serverIconSize = 48;
    int serverIconSpacing = 56;
    int serverIconX = 12;
    
    // Servidor 1 - Azul Discord
    HBRUSH hServer1 = CreateSolidBrush(COLOR_DISCORD_BLUE);
    hOldBrush = (HBRUSH)SelectObject(hdc, hServer1);
    Ellipse(hdc, serverIconX, serverIconY, serverIconX + serverIconSize, serverIconY + serverIconSize);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hServer1);
    
    // Indicador de servidor ativo (barra lateral)
    RECT activeIndicator = { 0, serverIconY + 4, 4, serverIconY + serverIconSize - 4 };
    HBRUSH hActive = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &activeIndicator, hActive);
    DeleteObject(hActive);
    
    // Servidor 2 - Verde
    HBRUSH hServer2 = CreateSolidBrush(RGB(67, 181, 129));
    hOldBrush = (HBRUSH)SelectObject(hdc, hServer2);
    Ellipse(hdc, serverIconX, serverIconY + serverIconSpacing, serverIconX + serverIconSize, serverIconY + serverIconSpacing + serverIconSize);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hServer2);
    
    // Servidor 3 - Laranja
    HBRUSH hServer3 = CreateSolidBrush(RGB(250, 168, 26));
    hOldBrush = (HBRUSH)SelectObject(hdc, hServer3);
    Ellipse(hdc, serverIconX, serverIconY + serverIconSpacing * 2, serverIconX + serverIconSize, serverIconY + serverIconSpacing * 2 + serverIconSize);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hServer3);
}

void UIRenderer::RenderChannelList(HDC hdc, const RECT& rect, const AppState& state) {
    // Fundo
    HBRUSH hBrush = CreateSolidBrush(COLOR_CHANNEL_LIST);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    
    // Barra superior do servidor
    RECT topBar = { rect.left, 0, rect.right, 48 };
    HBRUSH hTopBar = CreateSolidBrush(COLOR_CHANNEL_LIST);
    FillRect(hdc, &topBar, hTopBar);
    DeleteObject(hTopBar);
    
    // Separador superior
    HPEN hSepPen = CreatePen(PS_SOLID, 1, RGB(30, 33, 36));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hSepPen);
    MoveToEx(hdc, rect.left, 48, nullptr);
    LineTo(hdc, rect.right, 48);
    SelectObject(hdc, hOldPen);
    DeleteObject(hSepPen);
    
    // Nome do servidor com ícone de dropdown
    SetTextColor(hdc, COLOR_TEXT_PRIMARY);
    HFONT hServerFont = CreateFontA(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hServerFont);
    TextOutA(hdc, rect.left + 12, 16, "The Crew", 8);
    
    // Seta dropdown
    SetTextColor(hdc, COLOR_TEXT_MUTED);
    HFONT hIconFont = CreateFontA(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT hOldIconFont = (HFONT)SelectObject(hdc, hIconFont);
    TextOutA(hdc, rect.left + 80, 20, "v", 1);
    SelectObject(hdc, hOldIconFont);
    DeleteObject(hIconFont);
    SelectObject(hdc, hOldFont);
    DeleteObject(hServerFont);
    
    int yPos = 60;
    
    // Cabeçalho TEXT CHANNELS
    SetTextColor(hdc, COLOR_TEXT_MUTED);
    HFONT hHeaderFont = CreateFontA(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    hOldFont = (HFONT)SelectObject(hdc, hHeaderFont);
    TextOutA(hdc, rect.left + 12, yPos, "TEXT CHANNELS", 13);
    SelectObject(hdc, hOldFont);
    DeleteObject(hHeaderFont);
    yPos += 20;
    
    // Canais de texto
    SetTextColor(hdc, COLOR_TEXT_SECONDARY);
    HFONT hChannelFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    hOldFont = (HFONT)SelectObject(hdc, hChannelFont);
    
    // Canal selecionado (general) - destacado
    RECT channelRect = { rect.left, yPos - 2, rect.right, yPos + 22 };
    HBRUSH hSelected = CreateSolidBrush(RGB(57, 60, 67));
    FillRect(hdc, &channelRect, hSelected);
    DeleteObject(hSelected);
    
    // Barra lateral do canal selecionado
    RECT channelIndicator = { rect.left, yPos - 2, rect.left + 4, yPos + 22 };
    HBRUSH hChannelActive = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &channelIndicator, hChannelActive);
    DeleteObject(hChannelActive);
    
    SetTextColor(hdc, COLOR_TEXT_PRIMARY);
    TextOutA(hdc, rect.left + 12, yPos, "# general", 9);
    yPos += 24;
    
    // Outros canais
    SetTextColor(hdc, COLOR_TEXT_SECONDARY);
    TextOutA(hdc, rect.left + 12, yPos, "# announcements", 15);
    yPos += 24;
    TextOutA(hdc, rect.left + 12, yPos, "# welcome", 9);
    yPos += 24;
    TextOutA(hdc, rect.left + 12, yPos, "# off-topic", 11);
    yPos += 24;
    TextOutA(hdc, rect.left + 12, yPos, "# resource-share", 16);
    yPos += 32;
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hChannelFont);
    
    // Cabeçalho VOICE CHANNELS
    SetTextColor(hdc, COLOR_TEXT_MUTED);
    hOldFont = (HFONT)SelectObject(hdc, hHeaderFont);
    TextOutA(hdc, rect.left + 12, yPos, "VOICE CHANNELS", 14);
    SelectObject(hdc, hOldFont);
    yPos += 20;
    
    // Canais de voz com ícone
    SetTextColor(hdc, COLOR_TEXT_SECONDARY);
    hOldFont = (HFONT)SelectObject(hdc, hChannelFont);
    TextOutA(hdc, rect.left + 12, yPos, "wumpus-n-friends", 16);
    yPos += 24;
    TextOutA(hdc, rect.left + 12, yPos, "the-lounge", 10);
    SelectObject(hdc, hOldFont);
    DeleteObject(hHeaderFont);
    DeleteObject(hChannelFont);
}

void UIRenderer::RenderChatArea(HDC hdc, const RECT& rect, const AppState& state) {
    // Fundo principal
    HBRUSH hBrush = CreateSolidBrush(COLOR_MAIN_BG);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    
    // Barra superior do canal
    RECT topBar = { rect.left, 0, rect.right, 48 };
    HBRUSH hTopBar = CreateSolidBrush(COLOR_MAIN_BG);
    FillRect(hdc, &topBar, hTopBar);
    DeleteObject(hTopBar);
    
    // Ícone # do canal
    SetTextColor(hdc, COLOR_TEXT_MUTED);
    HFONT hIconFont = CreateFontA(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hIconFont);
    TextOutA(hdc, rect.left + 16, 14, "#", 1);
    SelectObject(hdc, hOldFont);
    DeleteObject(hIconFont);
    
    // Nome do canal
    SetTextColor(hdc, COLOR_TEXT_PRIMARY);
    HFONT hChannelFont = CreateFontA(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    hOldFont = (HFONT)SelectObject(hdc, hChannelFont);
    TextOutA(hdc, rect.left + 36, 16, "general", 7);
    SelectObject(hdc, hOldFont);
    DeleteObject(hChannelFont);
    
    // Separador superior
    HPEN hSepPen = CreatePen(PS_SOLID, 1, RGB(30, 33, 36));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hSepPen);
    MoveToEx(hdc, rect.left, 48, nullptr);
    LineTo(hdc, rect.right, 48);
    SelectObject(hdc, hOldPen);
    DeleteObject(hSepPen);
    
    // Mensagens do chat
    HFONT hRegularFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT hSmallFont = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    
    int chatY = 72;
    size_t maxMessages = 25;
    size_t startIdx = (state.chatHistory.size() > maxMessages) ? 
        (state.chatHistory.size() - maxMessages) : 0;
    
    for (size_t i = startIdx; i < state.chatHistory.size() && chatY < rect.bottom - 80; ++i) {
        const auto& msg = state.chatHistory[i];
        
        // Avatar - círculo colorido
        int avatarSize = 40;
        int avatarX = rect.left + 16;
        int avatarY = chatY;
        
        // Cor do avatar baseada no hash do username
        unsigned int hash = 0;
        for (char c : msg.username) {
            hash = hash * 31 + static_cast<unsigned char>(c);
        }
        BYTE r = static_cast<BYTE>((hash & 0xFF0000) >> 16);
        BYTE g = static_cast<BYTE>((hash & 0x00FF00) >> 8);
        BYTE b = static_cast<BYTE>(hash & 0x0000FF);
        
        // Garantir cores vibrantes
        if (r < 100) r = 100 + (r % 100);
        if (g < 100) g = 100 + (g % 100);
        if (b < 100) b = 100 + (b % 100);
        
        COLORREF avatarColor = RGB(r, g, b);
        
        HBRUSH hAvatar = CreateSolidBrush(avatarColor);
        HPEN hAvatarPen = CreatePen(PS_SOLID, 0, avatarColor);
        hOldPen = (HPEN)SelectObject(hdc, hAvatarPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hAvatar);
        Ellipse(hdc, avatarX, avatarY, avatarX + avatarSize, avatarY + avatarSize);
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hAvatar);
        DeleteObject(hAvatarPen);
        
        // Inicial do username no avatar
        SetTextColor(hdc, RGB(255, 255, 255));
        HFONT hAvatarFont = CreateFontA(18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
        hOldFont = (HFONT)SelectObject(hdc, hAvatarFont);
        char initial = msg.username.empty() ? '?' : static_cast<char>(toupper(msg.username[0]));
        char initialStr[2] = { initial, '\0' };
        RECT avatarTextRect = { avatarX, avatarY, avatarX + avatarSize, avatarY + avatarSize };
        DrawTextCentered(hdc, initialStr, avatarTextRect, hAvatarFont, RGB(255, 255, 255));
        SelectObject(hdc, hOldFont);
        DeleteObject(hAvatarFont);
        
        // Username
        int textX = avatarX + avatarSize + 12;
        SetTextColor(hdc, COLOR_TEXT_PRIMARY);
        hOldFont = (HFONT)SelectObject(hdc, hRegularFont);
        TextOutA(hdc, textX, chatY + 2, msg.username.c_str(), static_cast<int>(msg.username.length()));
        
        // Badge BOT
        if (msg.isBot) {
            // Fundo do badge BOT
            RECT botRect = { textX + static_cast<int>(msg.username.length() * 9) + 6, chatY + 2, 
                           textX + static_cast<int>(msg.username.length() * 9) + 30, chatY + 16 };
            HBRUSH hBotBg = CreateSolidBrush(COLOR_DISCORD_BLUE);
            FillRect(hdc, &botRect, hBotBg);
            DeleteObject(hBotBg);
            
            SetTextColor(hdc, RGB(255, 255, 255));
            HFONT hBotFont = CreateFontA(10, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
            HFONT hOldBotFont = (HFONT)SelectObject(hdc, hBotFont);
            TextOutA(hdc, textX + static_cast<int>(msg.username.length() * 9) + 9, chatY + 4, "BOT", 3);
            SelectObject(hdc, hOldBotFont);
            DeleteObject(hBotFont);
        }
        
        // Timestamp
        SetTextColor(hdc, COLOR_TEXT_MUTED);
        hOldFont = (HFONT)SelectObject(hdc, hSmallFont);
        int nameWidth = static_cast<int>(msg.username.length() * 9);
        int botWidth = msg.isBot ? 30 : 0;
        TextOutA(hdc, textX + nameWidth + botWidth + 8, chatY + 4, msg.timestamp.c_str(), static_cast<int>(msg.timestamp.length()));
        SelectObject(hdc, hOldFont);
        chatY += 22;
        
        // Mensagem
        SetTextColor(hdc, COLOR_TEXT_SECONDARY);
        hOldFont = (HFONT)SelectObject(hdc, hRegularFont);
        TextOutA(hdc, textX, chatY, msg.message.c_str(), static_cast<int>(msg.message.length()));
        chatY += 28;
        
        SelectObject(hdc, hOldFont);
    }
    
    DeleteObject(hRegularFont);
    DeleteObject(hSmallFont);
}

void UIRenderer::RenderInputBar(HDC hdc, const RECT& rect) {
    // Fundo da barra de input
    HBRUSH hBrush = CreateSolidBrush(COLOR_INPUT_BAR);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    
    // Separador superior
    HPEN hSepPen = CreatePen(PS_SOLID, 1, RGB(30, 33, 36));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hSepPen);
    MoveToEx(hdc, rect.left, rect.top, nullptr);
    LineTo(hdc, rect.right, rect.top);
    SelectObject(hdc, hOldPen);
    DeleteObject(hSepPen);
    
    // Campo de input (retângulo arredondado simulado)
    RECT inputRect = { rect.left + 16, rect.top + 8, rect.right - 16, rect.bottom - 8 };
    HBRUSH hInputBg = CreateSolidBrush(RGB(64, 68, 75));
    FillRect(hdc, &inputRect, hInputBg);
    DeleteObject(hInputBg);
    
    // Texto placeholder
    SetTextColor(hdc, COLOR_TEXT_MUTED);
    HFONT hInputFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hInputFont);
    TextOutA(hdc, rect.left + 24, rect.top + 16, "Message #general", 17);
    SelectObject(hdc, hOldFont);
    DeleteObject(hInputFont);
}

void UIRenderer::RenderUserBar(HDC hdc, const RECT& rect) {
    // Fundo
    HBRUSH hBrush = CreateSolidBrush(COLOR_USER_BAR);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    
    // Separador superior
    HPEN hSepPen = CreatePen(PS_SOLID, 1, RGB(30, 33, 36));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hSepPen);
    MoveToEx(hdc, rect.left, rect.top, nullptr);
    LineTo(hdc, rect.right, rect.top);
    SelectObject(hdc, hOldPen);
    DeleteObject(hSepPen);
    
    // Avatar do usuário
    int avatarSize = 32;
    int avatarX = rect.left + 8;
    int avatarY = rect.top + 10;
    
    HBRUSH hAvatar = CreateSolidBrush(COLOR_DISCORD_BLUE);
    HPEN hAvatarPen = CreatePen(PS_SOLID, 0, COLOR_DISCORD_BLUE);
    hOldPen = (HPEN)SelectObject(hdc, hAvatarPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hAvatar);
    Ellipse(hdc, avatarX, avatarY, avatarX + avatarSize, avatarY + avatarSize);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hAvatar);
    DeleteObject(hAvatarPen);
    
    // Inicial no avatar
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hAvatarFont = CreateFontA(14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hAvatarFont);
    RECT avatarTextRect = { avatarX, avatarY, avatarX + avatarSize, avatarY + avatarSize };
    DrawTextCentered(hdc, "U", avatarTextRect, hAvatarFont, RGB(255, 255, 255));
    SelectObject(hdc, hOldFont);
    DeleteObject(hAvatarFont);
    
    // Nome do usuário
    SetTextColor(hdc, COLOR_TEXT_PRIMARY);
    HFONT hUserFont = CreateFontA(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    hOldFont = (HFONT)SelectObject(hdc, hUserFont);
    TextOutA(hdc, avatarX + avatarSize + 8, avatarY + 4, "username", 8);
    SetTextColor(hdc, COLOR_TEXT_MUTED);
    TextOutA(hdc, avatarX + avatarSize + 8, avatarY + 18, "#0001", 5);
    SelectObject(hdc, hOldFont);
    DeleteObject(hUserFont);
}
