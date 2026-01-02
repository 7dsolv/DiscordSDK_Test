// Discord SDK Test Project
// Projeto de teste para integração do Discord Game SDK com Rich Presence

#include "src/window_handler.h"
#include "src/discord_client.h"
#include <iostream>
#include <memory>

// ID da aplicação Discord - SUBSTITUA PELO SEU APPLICATION ID
// Obtenha em: https://discord.com/developers/applications
const int64_t DISCORD_APPLICATION_ID = 123456789012345678; // ALTERE ESTE VALOR!

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Inicializar cliente Discord
    std::unique_ptr<DiscordClient> discordClient = std::make_unique<DiscordClient>();
    
    std::cout << "========================================" << std::endl;
    std::cout << "Discord SDK Test Project" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Tentar inicializar Discord SDK
    bool discordInitialized = false;
    if (DISCORD_APPLICATION_ID != 123456789012345678) {
        std::cout << "[INFO] Tentando inicializar Discord SDK..." << std::endl;
        discordInitialized = discordClient->Initialize(DISCORD_APPLICATION_ID);
        
        if (discordInitialized) {
            std::cout << "[SUCCESS] Discord SDK inicializado!" << std::endl;
            
            // Configurar callback para atualizações do usuário
            discordClient->SetOnUserUpdate([](const std::string& username, const std::string& userId) {
                std::cout << "[Discord] Usuário atualizado: " << username << " (ID: " << userId << ")" << std::endl;
            });
            
            // Definir Rich Presence inicial
            discordClient->UpdateRichPresence(
                "Testando Discord SDK",
                "Discord SDK Test Project",
                "discord_logo",
                "Discord SDK"
            );
        } else {
            std::cout << "[WARNING] Discord SDK não pôde ser inicializado." << std::endl;
            std::cout << "[INFO] Certifique-se de que:" << std::endl;
            std::cout << "  1. O Discord está aberto" << std::endl;
            std::cout << "  2. O APPLICATION_ID está correto" << std::endl;
            std::cout << "  3. O discord_game_sdk.dll está presente" << std::endl;
        }
    } else {
        std::cout << "[WARNING] APPLICATION_ID não configurado!" << std::endl;
        std::cout << "[INFO] Edite main.cpp e defina DISCORD_APPLICATION_ID" << std::endl;
        std::cout << "[INFO] Obtenha seu ID em: https://discord.com/developers/applications" << std::endl;
    }
    
    // Criar e inicializar handler de janela
    WindowHandler windowHandler;
    if (!windowHandler.Initialize(hInstance, nCmdShow, discordClient.get())) {
        std::cerr << "[ERROR] Falha ao criar janela!" << std::endl;
        return 1;
    }
    
    std::cout << "[INFO] Janela criada com sucesso!" << std::endl;
    std::cout << "[INFO] Pressione:" << std::endl;
    std::cout << "  C - Adicionar mensagem de chat" << std::endl;
    std::cout << "  V - Alternar canal de voz" << std::endl;
    std::cout << "  R - Alternar gravação" << std::endl;
    std::cout << "  ESC - Sair" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Executar loop principal
    windowHandler.Run();
    
    // Limpar Rich Presence ao sair
    if (discordInitialized && discordClient->IsConnected()) {
        discordClient->ClearRichPresence();
    }
    
    return 0;
}
