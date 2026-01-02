# 📊 Diagramas Simplificados - Visão Geral Rápida

Versões simplificadas dos diagramas principais para compreensão rápida.

---

## 🏗️ Arquitetura Simplificada

```
┌─────────────────────────────────────────────────┐
│           DiscordSDK_Test Application            │
├─────────────────────────────────────────────────┤
│                                                  │
│  ┌──────────────┐      ┌──────────────┐        │
│  │   main.cpp   │──────│DiscordClient  │        │
│  │  (Entry)     │      │  (SDK Wrapper)│        │
│  └──────┬───────┘      └──────┬────────┘        │
│         │                     │                  │
│         │                     │                  │
│  ┌──────▼──────────┐   ┌──────▼──────────┐       │
│  │ WindowHandler   │───│  UIRenderer    │       │
│  │ (Window Mgmt)   │   │  (UI Drawing)  │       │
│  └─────────────────┘   └────────────────┘       │
│                                                  │
└──────────────────┬──────────────────────────────┘
                   │
                   ▼
         ┌─────────────────┐
         │  Discord SDK    │
         │  discord::Core  │
         └────────┬────────┘
                  │
                  ▼
         ┌─────────────────┐
         │ Discord Desktop │
         │   (Rich Presence)│
         └─────────────────┘
```

---

## 🔄 Fluxo Principal Simplificado

```
INÍCIO
  │
  ├─► Criar DiscordClient
  │     │
  │     ├─► Inicializar SDK
  │     │     │
  │     │     └─► Criar Thread de Callbacks
  │     │
  │     └─► Obter Usuário
  │
  ├─► Criar WindowHandler
  │     │
  │     ├─► Registrar Classe de Janela
  │     ├─► Criar Janela
  │     └─► Mostrar Janela
  │
  └─► LOOP PRINCIPAL
        │
        ├─► Processar Mensagens Windows
        │     │
        │     ├─► WM_PAINT → Renderizar UI
        │     ├─► WM_KEYDOWN → Processar Teclas
        │     └─► WM_DESTROY → Sair
        │
        └─► Executar Callbacks Discord (60 FPS)
```

---

## ⌨️ Controles e Ações

```
Usuário Pressiona Tecla
         │
         ├─► 'C' → Adicionar Mensagem Chat
         │         └─► UpdateRichPresence("No canal #general")
         │
         ├─► 'V' → Alternar Canal de Voz
         │         └─► UpdateRichPresence("Em canal de voz")
         │
         ├─► 'R' → Alternar Gravação
         │         └─► Atualizar estado interno
         │
         └─► ESC → Fechar Aplicação
                 └─► ClearRichPresence() → Sair
```

---

## 🧵 Threads e Concorrência

```
┌─────────────────────┐
│   Main Thread       │
│                     │
│  • Message Loop     │
│  • UI Updates       │
│  • User Input       │
└─────────────────────┘
         │
         │ (periodic)
         ▼
┌─────────────────────┐
│  Callback Thread    │
│  (60 FPS)           │
│                     │
│  • RunCallbacks()   │
│  • Process Events   │
│  • Sleep(16ms)      │
└─────────────────────┘
```

---

## 📦 Estrutura de Arquivos

```
DiscordSDK_Test/
│
├── main.cpp              ← Entry Point
│
├── src/
│   ├── discord_client.*  ← SDK Wrapper
│   ├── window_handler.* ← Window Management
│   ├── ui_renderer.*     ← UI Rendering
│   └── types.h           ← Data Structures
│
└── External/
    └── discord-files/    ← Discord SDK
        ├── include/
        └── lib/
```

---

## 🔌 Integração Discord

```
Aplicação
    │
    │ UpdateRichPresence()
    ▼
DiscordClient
    │
    │ UpdateActivity()
    ▼
discord::Core
    │
    │ ActivityManager
    ▼
Discord Desktop
    │
    │ Rich Presence
    ▼
Perfil do Usuário
```

---

## 📊 Estados da Aplicação

```
[Não Inicializado]
       │
       ▼
[Inicializando SDK]
       │
       ├─► [Erro] ──► [Fim]
       │
       ▼
[SDK Conectado]
       │
       ▼
[Janela Criada]
       │
       ▼
[Rodando]
       │
       ├─► [Idle]
       ├─► [Chat Ativo]
       ├─► [Voice Ativo]
       └─► [Recording]
       │
       ▼
[Fechando]
       │
       ▼
[Fim]
```

---

**Versão Simplificada - Para referência rápida**
