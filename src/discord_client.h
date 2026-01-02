#pragma once

#include "../External/discord-files/include/discord.h"
#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include <functional>

class DiscordClient {
public:
    DiscordClient();
    ~DiscordClient();

    // Inicializar o cliente Discord
    bool Initialize(int64_t applicationId);
    
    // Atualizar Rich Presence
    void UpdateRichPresence(
        const std::string& state,
        const std::string& details,
        const std::string& largeImageKey = "",
        const std::string& largeImageText = "",
        const std::string& smallImageKey = "",
        const std::string& smallImageText = ""
    );
    
    // Limpar Rich Presence
    void ClearRichPresence();
    
    // Executar callbacks (deve ser chamado periodicamente)
    void RunCallbacks();
    
    // Verificar se está conectado
    bool IsConnected() const { return isConnected_; }
    
    // Obter informações do usuário
    std::string GetUsername() const { return username_; }
    std::string GetUserId() const { return userId_; }
    
    // Callback para quando o usuário é atualizado
    void SetOnUserUpdate(std::function<void(const std::string&, const std::string&)> callback);

private:
    std::unique_ptr<discord::Core> core_;
    std::atomic<bool> isConnected_;
    std::atomic<bool> shouldRun_;
    std::thread callbackThread_;
    
    std::string username_;
    std::string userId_;
    
    std::function<void(const std::string&, const std::string&)> onUserUpdate_;
    
    void CallbackThread();
    void InitializeUser();
};
