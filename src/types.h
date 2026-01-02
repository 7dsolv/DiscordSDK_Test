#pragma once

#include <string>
#include <vector>

// Estrutura de mensagem de chat
struct ChatMessage {
    std::string userId;
    std::string username;
    std::string message;
    std::string timestamp;
    std::string avatarColor;
    bool isBot = false;
};

// Estrutura de servidor/guilda
struct Guild {
    int id;
    std::string name;
    std::vector<std::string> channels;
    std::vector<std::string> voiceChannels;
};

// Estado da aplicação
struct AppState {
    std::vector<ChatMessage> chatHistory;
    std::vector<Guild> guilds;
    bool isVoiceActive = false;
    bool isRecording = false;
    int selectedGuildId = 0;
    int selectedChannelId = 0;
};
