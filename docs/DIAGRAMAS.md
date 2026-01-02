# 📊 Diagramas do Projeto - Discord SDK Test

Este documento contém todos os diagramas avançados do sistema, incluindo arquitetura, classes, sequência, componentes, fluxo de dados, estados e threads.

---

## 🏗️ 1. Diagrama de Arquitetura do Sistema

```mermaid
graph TB
    subgraph "Aplicação DiscordSDK_Test"
        Main[main.cpp<br/>Ponto de Entrada]
        
        subgraph "Camada de Apresentação"
            WH[WindowHandler<br/>Gerenciamento de Janela]
            UI[UIRenderer<br/>Renderização UI]
        end
        
        subgraph "Camada de Negócio"
            DC[DiscordClient<br/>Cliente Discord SDK]
            Types[types.h<br/>Estruturas de Dados]
        end
        
        subgraph "Camada de Dados"
            State[AppState<br/>Estado da Aplicação]
            Chat[ChatMessage<br/>Histórico de Chat]
        end
    end
    
    subgraph "Discord SDK"
        SDK[discord::Core<br/>SDK Principal]
        AM[ActivityManager<br/>Rich Presence]
        UM[UserManager<br/>Usuário]
        LM[LogManager<br/>Logs]
    end
    
    subgraph "Sistema Operacional"
        WinAPI[Windows API<br/>Win32]
        Thread[Thread Pool<br/>Callbacks]
    end
    
    Main --> WH
    Main --> DC
    
    WH --> UI
    WH --> State
    WH --> DC
    
    UI --> State
    UI --> WinAPI
    
    DC --> SDK
    DC --> Thread
    
    SDK --> AM
    SDK --> UM
    SDK --> LM
    
    State --> Chat
    
    style Main fill:#5865f2,stroke:#333,stroke-width:3px,color:#fff
    style DC fill:#5865f2,stroke:#333,stroke-width:2px,color:#fff
    style SDK fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
    style WH fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
    style UI fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
```

---

## 📐 2. Diagrama de Classes UML

```mermaid
classDiagram
    class DiscordClient {
        -unique_ptr~discord::Core~ core_
        -atomic~bool~ isConnected_
        -atomic~bool~ shouldRun_
        -thread callbackThread_
        -string username_
        -string userId_
        -function onUserUpdate_
        +DiscordClient()
        +~DiscordClient()
        +Initialize(int64_t) bool
        +UpdateRichPresence(...) void
        +ClearRichPresence() void
        +RunCallbacks() void
        +IsConnected() bool
        +GetUsername() string
        +GetUserId() string
        +SetOnUserUpdate(function) void
        -CallbackThread() void
        -InitializeUser() void
    }
    
    class WindowHandler {
        -HWND hwnd_
        -HINSTANCE hInstance_
        -DiscordClient* discordClient_
        -AppState state_
        +WindowHandler()
        +~WindowHandler()
        +Initialize(HINSTANCE, int, DiscordClient*) bool
        +Run() void
        +Shutdown() void
        +GetState() AppState&
        -WindowProc(HWND, UINT, WPARAM, LPARAM) LRESULT
        -GetInstance(HWND) WindowHandler*
        -HandleCommand(int) void
        -HandleKeyDown(WPARAM) void
        -AddChatMessage(string, string, bool) void
        -GetCurrentTime() string
    }
    
    class UIRenderer {
        +Render(HWND, AppState) void$
        -RenderServerList(HDC, RECT) void$
        -RenderChannelList(HDC, RECT, AppState) void$
        -RenderChatArea(HDC, RECT, AppState) void$
        -RenderInputBar(HDC, RECT) void$
        -RenderUserBar(HDC, RECT) void$
    }
    
    class AppState {
        +vector~ChatMessage~ chatHistory
        +vector~Guild~ guilds
        +bool isVoiceActive
        +bool isRecording
        +int selectedGuildId
        +int selectedChannelId
    }
    
    class ChatMessage {
        +string userId
        +string username
        +string message
        +string timestamp
        +string avatarColor
        +bool isBot
    }
    
    class Guild {
        +int id
        +string name
        +vector~string~ channels
        +vector~string~ voiceChannels
    }
    
    class discordCore {
        <<external>>
        +Create(int64_t, uint64_t, Core**) Result
        +RunCallbacks() void
        +SetLogHook(LogLevel, function) void
        +ActivityManager() ActivityManager&
        +UserManager() UserManager&
    }
    
    WindowHandler --> DiscordClient : uses
    WindowHandler --> AppState : contains
    WindowHandler --> UIRenderer : uses
    UIRenderer --> AppState : reads
    AppState --> ChatMessage : contains
    AppState --> Guild : contains
    DiscordClient --> discordCore : wraps
```

---

## 🔄 3. Diagrama de Sequência - Inicialização

```mermaid
sequenceDiagram
    participant Main as main.cpp
    participant DC as DiscordClient
    participant SDK as discord::Core
    participant Thread as CallbackThread
    participant WH as WindowHandler
    participant UI as UIRenderer
    participant WinAPI as Windows API
    
    Main->>DC: Initialize(applicationId)
    DC->>SDK: Core::Create(applicationId)
    SDK-->>DC: Result::Ok
    DC->>SDK: SetLogHook()
    DC->>SDK: GetCurrentUser()
    SDK-->>DC: User data
    DC->>DC: InitializeUser()
    DC->>Thread: Start CallbackThread
    DC-->>Main: true (initialized)
    
    Main->>WH: Initialize(hInstance, nCmdShow, discordClient)
    WH->>WinAPI: RegisterClass()
    WH->>WinAPI: CreateWindowEx()
    WinAPI-->>WH: HWND
    WH->>WH: AddChatMessage("System", ...)
    WH->>WinAPI: ShowWindow()
    WH-->>Main: true
    
    Main->>WH: Run()
    
    loop Message Loop
        WinAPI->>WH: WM_PAINT
        WH->>UI: Render(hwnd, state)
        UI->>UI: RenderServerList()
        UI->>UI: RenderChannelList()
        UI->>UI: RenderChatArea()
        UI->>UI: RenderInputBar()
        UI->>UI: RenderUserBar()
        UI-->>WH: Done
    end
    
    loop Callback Thread (60 FPS)
        Thread->>SDK: RunCallbacks()
        SDK-->>Thread: Processed
        Thread->>Thread: Sleep(16ms)
    end
```

---

## 🔄 4. Diagrama de Sequência - Atualização de Rich Presence

```mermaid
sequenceDiagram
    participant User as Usuário
    participant WH as WindowHandler
    participant DC as DiscordClient
    participant SDK as discord::Core
    participant AM as ActivityManager
    participant Discord as Discord Desktop
    
    User->>WH: Pressiona 'C' (Chat)
    WH->>WH: HandleKeyDown('C')
    WH->>WH: HandleCommand(1001)
    WH->>WH: AddChatMessage(...)
    WH->>DC: UpdateRichPresence("No canal #general", ...)
    
    DC->>DC: Verifica isConnected_
    DC->>AM: Create Activity{}
    DC->>AM: SetType(Playing)
    DC->>AM: SetState("No canal #general")
    DC->>AM: SetDetails("Testando Discord SDK")
    DC->>AM: SetTimestamps(now)
    DC->>AM: SetAssets(...)
    
    DC->>AM: UpdateActivity(activity, callback)
    AM->>Discord: Send Rich Presence Update
    Discord-->>AM: Result::Ok
    AM-->>DC: Callback(Result::Ok)
    DC->>DC: Log success
    DC-->>WH: Done
    
    WH->>WH: InvalidateRect()
    WH->>UI: Render()
    UI-->>User: UI atualizada
    Discord-->>User: Rich Presence atualizado no perfil
```

---

## 🧩 5. Diagrama de Componentes

```mermaid
graph LR
    subgraph "Componente: Aplicação Principal"
        Main[main.cpp]
    end
    
    subgraph "Componente: Gerenciamento de Janela"
        WH[WindowHandler]
        WH_Impl[window_handler.cpp]
        WH --> WH_Impl
    end
    
    subgraph "Componente: Cliente Discord"
        DC[DiscordClient]
        DC_Impl[discord_client.cpp]
        DC --> DC_Impl
    end
    
    subgraph "Componente: Renderização UI"
        UI[UIRenderer]
        UI_Impl[ui_renderer.cpp]
        UI --> UI_Impl
    end
    
    subgraph "Componente: Estruturas de Dados"
        Types[types.h]
        AppState[AppState]
        ChatMsg[ChatMessage]
        Guild[Guild]
        Types --> AppState
        Types --> ChatMsg
        Types --> Guild
    end
    
    subgraph "Componente: Discord SDK"
        SDK[discord.h]
        Core[discord::Core]
        SDK --> Core
    end
    
    subgraph "Componente: Sistema Operacional"
        WinAPI[Windows API]
        ThreadLib[std::thread]
    end
    
    Main --> WH
    Main --> DC
    WH --> UI
    WH --> Types
    WH --> DC
    DC --> SDK
    DC --> ThreadLib
    UI --> Types
    UI --> WinAPI
    
    style Main fill:#5865f2,stroke:#333,stroke-width:3px,color:#fff
    style DC fill:#5865f2,stroke:#333,stroke-width:2px,color:#fff
    style SDK fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
    style WH fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
    style UI fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
```

---

## 🌊 6. Diagrama de Fluxo de Dados

```mermaid
flowchart TD
    Start([Início da Aplicação]) --> Init[Inicializar DiscordClient]
    Init --> CheckID{Application ID<br/>válido?}
    
    CheckID -->|Não| Warn[Mostrar Warning]
    CheckID -->|Sim| CreateCore[Criar discord::Core]
    
    CreateCore --> CheckCore{Core criado<br/>com sucesso?}
    CheckCore -->|Não| Error[Mostrar Erro]
    CheckCore -->|Sim| SetupLog[Configurar Log Hook]
    
    SetupLog --> InitUser[Inicializar Usuário]
    InitUser --> StartThread[Iniciar Callback Thread]
    StartThread --> InitWindow[Inicializar WindowHandler]
    
    InitWindow --> RegisterClass[Registrar Classe de Janela]
    RegisterClass --> CreateWindow[Criar Janela]
    CreateWindow --> ShowWindow[Mostrar Janela]
    ShowWindow --> MessageLoop[Loop de Mensagens]
    
    MessageLoop --> CheckMsg{Tipo de<br/>Mensagem?}
    
    CheckMsg -->|WM_PAINT| Render[UIRenderer::Render]
    CheckMsg -->|WM_KEYDOWN| HandleKey[HandleKeyDown]
    CheckMsg -->|WM_DESTROY| Cleanup[Limpar Recursos]
    CheckMsg -->|Outras| Default[DefWindowProc]
    
    HandleKey --> KeyC{C == 'C'?}
    HandleKey --> KeyV{C == 'V'?}
    HandleKey --> KeyR{C == 'R'?}
    HandleKey --> KeyESC{C == ESC?}
    
    KeyC -->|Sim| UpdateRP1[UpdateRichPresence<br/>Chat]
    KeyV -->|Sim| UpdateRP2[UpdateRichPresence<br/>Voice]
    KeyR -->|Sim| ToggleRec[Alternar Gravação]
    KeyESC -->|Sim| Cleanup
    
    UpdateRP1 --> Invalidate[InvalidateRect]
    UpdateRP2 --> Invalidate
    ToggleRec --> Invalidate
    
    Render --> RenderServer[RenderServerList]
    Render --> RenderChannel[RenderChannelList]
    Render --> RenderChat[RenderChatArea]
    Render --> RenderInput[RenderInputBar]
    Render --> RenderUser[RenderUserBar]
    
    RenderServer --> MessageLoop
    RenderChannel --> MessageLoop
    RenderChat --> MessageLoop
    RenderInput --> MessageLoop
    RenderUser --> MessageLoop
    
    Invalidate --> MessageLoop
    Default --> MessageLoop
    
    Cleanup --> ClearRP[ClearRichPresence]
    ClearRP --> StopThread[Parar Callback Thread]
    StopThread --> DestroyWindow[DestroyWindow]
    DestroyWindow --> End([Fim])
    
    Error --> InitWindow
    Warn --> InitWindow
    
    style Start fill:#5865f2,stroke:#333,stroke-width:3px,color:#fff
    style End fill:#f04747,stroke:#333,stroke-width:3px,color:#fff
    style CreateCore fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
    style UpdateRP1 fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
    style UpdateRP2 fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
```

---

## 🔄 7. Diagrama de Estados

```mermaid
stateDiagram-v2
    [*] --> NãoInicializado
    
    NãoInicializado --> Inicializando: Initialize()
    Inicializando --> Inicializado: Core criado
    Inicializando --> Erro: Falha ao criar Core
    
    Inicializado --> Conectado: Usuário obtido
    Conectado --> Ativo: Rich Presence definido
    
    state Ativo {
        [*] --> Idle
        Idle --> Chat: Pressiona 'C'
        Chat --> Idle: Rich Presence atualizado
        Idle --> Voice: Pressiona 'V'
        Voice --> Idle: Rich Presence atualizado
        Idle --> Recording: Pressiona 'R'
        Recording --> Idle: Status alternado
    }
    
    Ativo --> Limpando: WM_DESTROY
    Limpando --> Desconectado: ClearRichPresence()
    Desconectado --> [*]: Thread parada
    
    Erro --> [*]
    
    note right of Inicializado
        Thread de callbacks
        iniciada (60 FPS)
    end note
    
    note right of Ativo
        Estado principal
        da aplicação
    end note
```

---

## 🧵 8. Diagrama de Threads e Concorrência

```mermaid
sequenceDiagram
    participant Main as Main Thread
    participant DC as DiscordClient
    participant CBThread as Callback Thread
    participant SDK as Discord SDK
    participant WH as Window Thread
    participant UI as UI Renderer
    
    Main->>DC: Initialize()
    Main->>DC: Start CallbackThread
    DC->>CBThread: std::thread(&CallbackThread)
    
    par Main Thread
        Main->>WH: Initialize()
        Main->>WH: Run()
        loop Message Loop
            WH->>UI: Render()
            UI-->>WH: Done
            WH->>DC: RunCallbacks() (periodic)
        end
    and Callback Thread
        loop 60 FPS (16ms)
            CBThread->>SDK: RunCallbacks()
            SDK->>SDK: Process Events
            SDK-->>CBThread: Done
            CBThread->>CBThread: Sleep(16ms)
        end
    end
    
    Main->>DC: UpdateRichPresence()
    DC->>SDK: UpdateActivity()
    SDK-->>DC: Callback(Result)
    
    Main->>DC: ~DiscordClient()
    DC->>CBThread: shouldRun_ = false
    CBThread->>CBThread: Exit loop
    CBThread-->>DC: Thread joined
```

---

## 📦 9. Diagrama de Dependências

```mermaid
graph TD
    subgraph "Arquivos Fonte"
        Main[main.cpp]
        WH_H[window_handler.h]
        WH_C[window_handler.cpp]
        DC_H[discord_client.h]
        DC_C[discord_client.cpp]
        UI_H[ui_renderer.h]
        UI_C[ui_renderer.cpp]
        Types[types.h]
    end
    
    subgraph "Bibliotecas Externas"
        DiscordSDK[discord.h<br/>Discord SDK]
        WinAPI[windows.h<br/>Windows API]
        STL[std::<br/>C++ STL]
    end
    
    Main --> WH_H
    Main --> DC_H
    
    WH_H --> Types
    WH_H --> DC_H
    WH_H --> WinAPI
    WH_C --> WH_H
    WH_C --> UI_H
    
    DC_H --> DiscordSDK
    DC_H --> STL
    DC_C --> DC_H
    
    UI_H --> Types
    UI_H --> WinAPI
    UI_C --> UI_H
    
    Types --> STL
    
    style Main fill:#5865f2,stroke:#333,stroke-width:3px,color:#fff
    style DiscordSDK fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
    style WinAPI fill:#43b581,stroke:#333,stroke-width:2px,color:#fff
```

---

## 🎯 10. Diagrama de Casos de Uso

```mermaid
graph LR
    User((Usuário))
    
    subgraph "Casos de Uso"
        UC1[Inicializar<br/>Discord SDK]
        UC2[Atualizar<br/>Rich Presence]
        UC3[Visualizar<br/>Interface]
        UC4[Enviar Mensagem<br/>de Chat]
        UC5[Alternar Canal<br/>de Voz]
        UC6[Alternar<br/>Gravação]
        UC7[Fechar<br/>Aplicação]
    end
    
    subgraph "Sistema"
        S1[DiscordClient]
        S2[WindowHandler]
        S3[UIRenderer]
    end
    
    User --> UC1
    User --> UC2
    User --> UC3
    User --> UC4
    User --> UC5
    User --> UC6
    User --> UC7
    
    UC1 --> S1
    UC2 --> S1
    UC3 --> S3
    UC4 --> S2
    UC5 --> S2
    UC6 --> S2
    UC7 --> S2
    
    S1 --> Discord[Discord Desktop]
    S2 --> S1
    S2 --> S3
    
    style User fill:#5865f2,stroke:#333,stroke-width:3px,color:#fff
    style Discord fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
```

---

## 🔌 11. Diagrama de Integração com Discord SDK

```mermaid
graph TB
    subgraph "Aplicação"
        App[DiscordSDK_Test]
        DC[DiscordClient]
    end
    
    subgraph "Discord SDK Core"
        Core[discord::Core]
        AM[ActivityManager]
        UM[UserManager]
        LM[LogManager]
    end
    
    subgraph "Discord Desktop"
        Client[Discord Client]
        RP[Rich Presence<br/>Display]
        User[User Profile]
    end
    
    App --> DC
    DC --> Core
    
    Core --> AM
    Core --> UM
    Core --> LM
    
    AM -->|UpdateActivity| Client
    UM -->|GetCurrentUser| Client
    LM -->|Log Messages| Client
    
    Client --> RP
    Client --> User
    
    RP -.->|Exibe no perfil| User
    
    style App fill:#5865f2,stroke:#333,stroke-width:3px,color:#fff
    style Core fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
    style Client fill:#7289da,stroke:#333,stroke-width:2px,color:#fff
```

---

## 📊 12. Diagrama de Ciclo de Vida da Aplicação

```mermaid
stateDiagram-v2
    [*] --> Startup
    
    Startup --> LoadingSDK: Carregar DLL
    LoadingSDK --> CreatingCore: DLL carregado
    CreatingCore --> Initializing: Core criado
    Initializing --> CreatingWindow: SDK inicializado
    CreatingWindow --> Running: Janela criada
    
    state Running {
        [*] --> Idle
        Idle --> ProcessingInput: Input do usuário
        ProcessingInput --> UpdatingRP: Atualizar Rich Presence
        UpdatingRP --> Rendering: Invalidar janela
        Rendering --> Idle: Render completo
        
        Idle --> ProcessingCallbacks: Callback thread (60 FPS)
        ProcessingCallbacks --> Idle: Callbacks processados
    }
    
    Running --> ShuttingDown: WM_DESTROY
    ShuttingDown --> ClearingRP: Limpar Rich Presence
    ClearingRP --> StoppingThread: Parar thread
    StoppingThread --> DestroyingWindow: Destruir janela
    DestroyingWindow --> [*]
    
    LoadingSDK --> Error: Falha ao carregar
    CreatingCore --> Error: Falha ao criar
    Initializing --> Error: Falha ao inicializar
    CreatingWindow --> Error: Falha ao criar janela
    Error --> [*]
```

---

## 📝 Notas sobre os Diagramas

### Diagrama de Arquitetura
- Mostra a separação em camadas (Apresentação, Negócio, Dados)
- Demonstra a integração com Discord SDK
- Indica dependências do sistema operacional

### Diagrama de Classes
- Representa todas as classes principais
- Mostra relacionamentos (composição, agregação, uso)
- Inclui métodos públicos e privados

### Diagrama de Sequência
- Detalha o fluxo de inicialização
- Mostra interações entre componentes
- Inclui loops e threads

### Diagrama de Componentes
- Organiza o sistema em componentes reutilizáveis
- Mostra dependências entre componentes
- Separa implementação de interface

### Diagrama de Fluxo de Dados
- Mostra todo o fluxo de execução
- Inclui decisões e loops
- Indica tratamento de erros

### Diagrama de Estados
- Representa estados da aplicação
- Mostra transições entre estados
- Inclui estados aninhados

### Diagrama de Threads
- Demonstra concorrência
- Mostra sincronização
- Indica comunicação entre threads

---

**Última atualização:** Dezembro 2024  
**Versão dos Diagramas:** 1.0
