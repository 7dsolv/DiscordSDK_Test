#include "discord_client.h"
#include <iostream>
#include <chrono>

DiscordClient::DiscordClient() 
    : isConnected_(false)
    , shouldRun_(false)
{
}

DiscordClient::~DiscordClient() {
    shouldRun_ = false;
    if (callbackThread_.joinable()) {
        callbackThread_.join();
    }
    core_.reset();
}

bool DiscordClient::Initialize(int64_t applicationId) {
    discord::Core* corePtr = nullptr;
    discord::Result result = discord::Core::Create(
        applicationId,
        static_cast<uint64_t>(discord::CreateFlags::NoRequireDiscord),
        &corePtr
    );
    
    if (result != discord::Result::Ok) {
        std::cerr << "[Discord] Falha ao criar core: " << static_cast<int>(result) << std::endl;
        return false;
    }
    
    core_.reset(corePtr);
    
    // Configurar log hook
    core_->SetLogHook(
        discord::LogLevel::Info,
        [](discord::LogLevel level, const char* message) {
            std::cout << "[Discord] " << message << std::endl;
        }
    );
    
    // Inicializar informações do usuário
    InitializeUser();
    
    // Configurar callback para atualizações do usuário
    core_->UserManager().OnCurrentUserUpdate.Connect([this]() {
        InitializeUser();
    });
    
    isConnected_ = true;
    shouldRun_ = true;
    
    // Iniciar thread de callbacks
    callbackThread_ = std::thread(&DiscordClient::CallbackThread, this);
    
    std::cout << "[Discord] Cliente inicializado com sucesso!" << std::endl;
    return true;
}

void DiscordClient::InitializeUser() {
    if (!core_) return;
    
    auto& userManager = core_->UserManager();
    discord::User currentUser;
    
    auto result = userManager.GetCurrentUser(&currentUser);
    if (result == discord::Result::Ok) {
        username_ = currentUser.GetUsername();
        userId_ = std::to_string(currentUser.GetId());
        
        if (onUserUpdate_) {
            onUserUpdate_(username_, userId_);
        }
        
        std::cout << "[Discord] Usuário: " << username_ << " (ID: " << userId_ << ")" << std::endl;
    }
}

void DiscordClient::UpdateRichPresence(
    const std::string& state,
    const std::string& details,
    const std::string& largeImageKey,
    const std::string& largeImageText,
    const std::string& smallImageKey,
    const std::string& smallImageText
) {
    if (!core_ || !isConnected_) {
        return;
    }
    
    discord::Activity activity{};
    activity.SetType(discord::ActivityType::Playing);
    activity.SetState(state.c_str());
    activity.SetDetails(details.c_str());
    
    // Configurar timestamps
    auto& timestamps = activity.GetTimestamps();
    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    timestamps.SetStart(static_cast<discord::Timestamp>(now));
    
    // Configurar assets (imagens)
    auto& assets = activity.GetAssets();
    if (!largeImageKey.empty()) {
        assets.SetLargeImage(largeImageKey.c_str());
        if (!largeImageText.empty()) {
            assets.SetLargeText(largeImageText.c_str());
        }
    }
    if (!smallImageKey.empty()) {
        assets.SetSmallImage(smallImageKey.c_str());
        if (!smallImageText.empty()) {
            assets.SetSmallText(smallImageText.c_str());
        }
    }
    
    // Atualizar atividade
    core_->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        if (result == discord::Result::Ok) {
            std::cout << "[Discord] Rich Presence atualizado com sucesso!" << std::endl;
        } else {
            std::cerr << "[Discord] Erro ao atualizar Rich Presence: " << static_cast<int>(result) << std::endl;
        }
    });
}

void DiscordClient::ClearRichPresence() {
    if (!core_ || !isConnected_) {
        return;
    }
    
    core_->ActivityManager().ClearActivity([](discord::Result result) {
        if (result == discord::Result::Ok) {
            std::cout << "[Discord] Rich Presence limpo!" << std::endl;
        }
    });
}

void DiscordClient::RunCallbacks() {
    if (core_ && isConnected_) {
        core_->RunCallbacks();
    }
}

void DiscordClient::SetOnUserUpdate(std::function<void(const std::string&, const std::string&)> callback) {
    onUserUpdate_ = callback;
}

void DiscordClient::CallbackThread() {
    while (shouldRun_) {
        RunCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
}
