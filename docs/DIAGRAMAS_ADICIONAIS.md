# 📊 Diagramas Adicionais - Discord SDK Test

Diagramas complementares para análise avançada do sistema.

---

## 🚀 1. Diagrama de Deployment

```mermaid
graph TB
    subgraph "Ambiente de Desenvolvimento"
        Dev[Developer Machine]
        VS[Visual Studio]
        Git[Git Repository]
    end
    
    subgraph "Build Process"
        Source[Source Files]
        Compiler[MSVC Compiler]
        Linker[Linker]
        DLL[discord_game_sdk.dll]
    end
    
    subgraph "Output"
        EXE[DiscordSDK_Test.exe]
        OutputDir[bin/Release/]
    end
    
    subgraph "Runtime Environment"
        Windows[Windows 10/11]
        Discord[Discord Desktop]
        User[End User]
    end
    
    Dev --> VS
    VS --> Source
    Source --> Compiler
    Compiler --> Linker
    Linker --> DLL
    Linker --> EXE
    EXE --> OutputDir
    
    OutputDir --> Windows
    Windows --> Discord
    Discord --> User
    
    Git --> Source
    
    style Dev fill:#5865f2,stroke:#333,stroke-width:2px,color:#fff
    style EXE fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
    style Discord fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
```

---

## 🌐 2. Diagrama de Rede e Comunicação

```mermaid
sequenceDiagram
    participant App as Aplicação
    participant SDK as Discord SDK
    participant IPC as IPC Pipe
    participant Discord as Discord Desktop
    participant API as Discord API
    
    App->>SDK: Initialize(applicationId)
    SDK->>IPC: Criar pipe nomeado
    IPC->>Discord: Conectar ao processo Discord
    Discord-->>IPC: Conexão estabelecida
    IPC-->>SDK: Conectado
    SDK-->>App: Initialized = true
    
    loop Rich Presence Updates
        App->>SDK: UpdateRichPresence(...)
        SDK->>IPC: Enviar dados via pipe
        IPC->>Discord: Transmitir dados
        Discord->>API: Atualizar via API
        API-->>Discord: Confirmação
        Discord-->>IPC: Callback
        IPC-->>SDK: Result
        SDK-->>App: Callback executado
    end
    
    loop Callbacks (60 FPS)
        SDK->>IPC: RunCallbacks()
        IPC->>Discord: Verificar eventos
        Discord-->>IPC: Eventos disponíveis
        IPC-->>SDK: Processar eventos
        SDK->>App: Trigger callbacks
    end
```

---

## ⏱️ 3. Diagrama de Timing

```mermaid
gantt
    title Ciclo de Vida da Aplicação
    dateFormat X
    axisFormat %L ms
    
    section Inicialização
    Carregar DLL           :0, 50
    Criar Core             :50, 100
    Obter Usuário          :100, 150
    Iniciar Thread         :150, 200
    Criar Janela           :200, 250
    Mostrar Janela         :250, 300
    
    section Execução
    Primeiro Render        :300, 350
    Message Loop           :350, 10000
    Callback Thread        :200, 10000
    
    section Atualização RP
    Usuário pressiona 'C'  :1000, 1050
    UpdateRichPresence     :1050, 1100
    Enviar para Discord    :1100, 1200
    Receber Callback       :1200, 1250
    Invalidar Janela       :1250, 1300
    Renderizar             :1300, 1350
    
    section Shutdown
    WM_DESTROY             :10000, 10050
    ClearRichPresence      :10050, 10100
    Parar Thread           :10100, 10150
    Destruir Janela        :10150, 10200
```

---

## 🔐 4. Diagrama de Segurança e Permissões

```mermaid
graph TB
    subgraph "Aplicação"
        App[DiscordSDK_Test.exe]
        SDK[Discord SDK]
    end
    
    subgraph "Sistema Operacional"
        Perms[Permissões]
        IPC[IPC Security]
        File[File System]
    end
    
    subgraph "Discord Desktop"
        Discord[Discord Process]
        RP[Rich Presence]
    end
    
    App -->|Require| Perms
    Perms -->|Check| IPC
    IPC -->|Allow| Discord
    
    SDK -->|Connect| IPC
    IPC -->|Validate| Discord
    
    Discord -->|Read| File
    Discord -->|Write| RP
    
    style App fill:#5865f2,stroke:#333,stroke-width:2px,color:#fff
    style Discord fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
    style Perms fill:#f04747,stroke:#333,stroke-width:2px,color:#fff
```

---

## 📈 5. Diagrama de Performance

```mermaid
graph LR
    subgraph "Thread Principal"
        ML[Message Loop<br/>~60 FPS]
        UI[UI Render<br/>~16ms]
    end
    
    subgraph "Thread de Callbacks"
        CB[Callback Thread<br/>60 FPS]
        RP[Rich Presence<br/>~16ms]
    end
    
    subgraph "Operações"
        Fast[Operações Rápidas<br/>< 1ms]
        Medium[Operações Médias<br/>1-10ms]
        Slow[Operações Lentas<br/>> 10ms]
    end
    
    ML --> UI
    UI --> Fast
    UI --> Medium
    
    CB --> RP
    RP --> Fast
    
    Slow -.->|Evitar| UI
    Slow -.->|Evitar| RP
    
    style ML fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
    style CB fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
    style Fast fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
    style Slow fill:#f04747,stroke:#333,stroke-width:2px,color:#fff
```

---

## 🔄 6. Diagrama de Atualização de Estado

```mermaid
stateDiagram-v2
    [*] --> StateInitial
    
    StateInitial --> StateIdle: App iniciada
    
    StateIdle --> StateChat: Key 'C'
    StateChat --> StateIdle: RP atualizado
    
    StateIdle --> StateVoice: Key 'V'
    StateVoice --> StateIdle: RP atualizado
    
    StateIdle --> StateRecording: Key 'R'
    StateRecording --> StateIdle: Status alternado
    
    state StateChat {
        [*] --> UpdatingRP
        UpdatingRP --> WaitingCallback
        WaitingCallback --> RPUpdated
        RPUpdated --> [*]
    }
    
    state StateVoice {
        [*] --> TogglingVoice
        TogglingVoice --> UpdatingRP
        UpdatingRP --> VoiceActive
        VoiceActive --> [*]
    }
    
    StateIdle --> [*]: ESC pressed
```

---

## 🧩 7. Diagrama de Módulos e Responsabilidades

```mermaid
mindmap
  root((DiscordSDK Test))
    DiscordClient
      SDK Integration
      Rich Presence
      User Management
      Callback Thread
    WindowHandler
      Window Creation
      Message Loop
      Input Handling
      State Management
    UIRenderer
      Server List
      Channel List
      Chat Area
      Input Bar
      User Bar
    Types
      AppState
      ChatMessage
      Guild
    Main
      Initialization
      Coordination
      Cleanup
```

---

## 📊 8. Diagrama de Dados - Rich Presence

```mermaid
erDiagram
    DISCORD_CLIENT ||--o{ RICH_PRESENCE : updates
    RICH_PRESENCE ||--|| ACTIVITY : contains
    ACTIVITY ||--|| TIMESTAMPS : has
    ACTIVITY ||--|| ASSETS : has
    ACTIVITY ||--|| BUTTONS : has
    
    DISCORD_CLIENT {
        int64_t applicationId
        bool isConnected
        string username
        string userId
    }
    
    RICH_PRESENCE {
        string state
        string details
        ActivityType type
    }
    
    ACTIVITY {
        string state
        string details
        ActivityType type
    }
    
    TIMESTAMPS {
        int64_t start
        int64_t end
    }
    
    ASSETS {
        string largeImage
        string largeText
        string smallImage
        string smallText
    }
    
    BUTTONS {
        string label
        string url
    }
```

---

## 🔧 9. Diagrama de Build e Compilação

```mermaid
graph TB
    subgraph "Source Files"
        CPP[*.cpp files]
        H[*.h files]
    end
    
    subgraph "Preprocessing"
        Preprocessor[C++ Preprocessor]
        Expanded[Expanded Code]
    end
    
    subgraph "Compilation"
        Compiler[MSVC Compiler]
        OBJ[*.obj files]
    end
    
    subgraph "Linking"
        Linker[Linker]
        LIB[discord_game_sdk.dll.lib]
        EXE[DiscordSDK_Test.exe]
    end
    
    subgraph "Post-Build"
        CopyDLL[Copy DLL]
        Output[bin/Release/]
    end
    
    CPP --> Preprocessor
    H --> Preprocessor
    Preprocessor --> Expanded
    Expanded --> Compiler
    Compiler --> OBJ
    OBJ --> Linker
    LIB --> Linker
    Linker --> EXE
    EXE --> CopyDLL
    CopyDLL --> Output
    
    style CPP fill:#5865f2,stroke:#333,stroke-width:2px,color:#fff
    style EXE fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
```

---

## 🎮 10. Diagrama de Interação do Usuário

```mermaid
graph TD
    User[Usuário]
    
    User -->|Visualiza| UI[Interface Gráfica]
    User -->|Pressiona 'C'| ActionC[Ação: Chat]
    User -->|Pressiona 'V'| ActionV[Ação: Voice]
    User -->|Pressiona 'R'| ActionR[Ação: Recording]
    User -->|Pressiona ESC| ActionESC[Ação: Sair]
    
    ActionC --> UpdateUI[Atualizar UI]
    ActionC --> UpdateRP[Atualizar Rich Presence]
    
    ActionV --> ToggleVoice[Alternar Voice]
    ActionV --> UpdateRP
    
    ActionR --> ToggleRec[Alternar Recording]
    ActionR --> UpdateUI
    
    ActionESC --> Cleanup[Limpar e Sair]
    
    UpdateRP --> Discord[Discord Desktop]
    Discord -->|Exibe| UserProfile[Perfil do Usuário]
    
    UpdateUI -->|Renderiza| UI
    
    style User fill:#5865f2,stroke:#333,stroke-width:3px,color:#fff
    style Discord fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
```

---

## 🔍 11. Diagrama de Debug e Logging

```mermaid
graph LR
    subgraph "Aplicação"
        App[DiscordSDK_Test]
        Log[Logging System]
    end
    
    subgraph "Outputs"
        Console[Console Output]
        File[Log File]
        Debugger[Debugger]
    end
    
    subgraph "Discord SDK"
        SDK[discord::Core]
        SDKLog[SDK Logs]
    end
    
    App --> Log
    Log --> Console
    Log --> File
    Log --> Debugger
    
    SDK --> SDKLog
    SDKLog --> Console
    
    style App fill:#5865f2,stroke:#333,stroke-width:2px,color:#fff
    style SDK fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
    style Console fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
```

---

## 📦 12. Diagrama de Estrutura de Diretórios

```mermaid
graph TD
    Root[DiscordSDK_Test/]
    
    Root --> Src[src/]
    Root --> Ext[External/]
    Root --> Bin[bin/]
    Root --> Docs[docs/]
    Root --> Main[main.cpp]
    
    Src --> DC[discord_client.*]
    Src --> WH[window_handler.*]
    Src --> UI[ui_renderer.*]
    Src --> Types[types.h]
    
    Ext --> SDK[discord-files/]
    SDK --> Inc[include/]
    SDK --> Lib[lib/]
    
    Bin --> Release[Release/]
    Release --> EXE[DiscordSDK_Test.exe]
    Release --> DLL[discord_game_sdk.dll]
    
    Docs --> Diag[DIAGRAMAS.md]
    Docs --> Readme[README_DIAGRAMAS.md]
    
    style Root fill:#5865f2,stroke:#333,stroke-width:3px,color:#fff
    style Src fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
    style Ext fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
```

---

## 🎯 13. Diagrama de Decisão - Inicialização

```mermaid
flowchart TD
    Start([Início]) --> LoadDLL{Discord SDK<br/>DLL carregado?}
    
    LoadDLL -->|Não| Error1[Erro: DLL não encontrada]
    LoadDLL -->|Sim| CheckAppID{Application ID<br/>válido?}
    
    CheckAppID -->|Não| Warn1[Warning: ID não configurado]
    CheckAppID -->|Sim| CreateCore{Criar<br/>discord::Core}
    
    CreateCore -->|Falha| Error2[Erro: Falha ao criar Core]
    CreateCore -->|Sucesso| CheckDiscord{Discord<br/>está aberto?}
    
    CheckDiscord -->|Não| Warn2[Warning: Discord não detectado]
    CheckDiscord -->|Sim| GetUser{Obter<br/>usuário}
    
    GetUser -->|Falha| Warn3[Warning: Usuário não disponível]
    GetUser -->|Sucesso| StartThread[Iniciar Callback Thread]
    
    StartThread --> CreateWindow{Criar<br/>Janela}
    
    CreateWindow -->|Falha| Error3[Erro: Falha ao criar janela]
    CreateWindow -->|Sucesso| Success[Inicialização Completa]
    
    Error1 --> End([Fim])
    Error2 --> End
    Error3 --> End
    Warn1 --> CreateWindow
    Warn2 --> CreateWindow
    Warn3 --> CreateWindow
    Success --> Run([Executar Aplicação])
    
    style Start fill:#5865f2,stroke:#333,stroke-width:3px,color:#fff
    style Success fill:#43b581,stroke:#333,stroke-width:3px,color:#fff
    style Error1 fill:#f04747,stroke:#333,stroke-width:2px,color:#fff
    style Error2 fill:#f04747,stroke:#333,stroke-width:2px,color:#fff
    style Error3 fill:#f04747,stroke:#333,stroke-width:2px,color:#fff
```

---

**Diagramas Adicionais - Para análise avançada**
