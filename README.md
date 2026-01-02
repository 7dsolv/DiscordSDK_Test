https://www.mediafire.com/file/tvnp8r8zr5onv0g/DiscordSDK_Test.rar 


# 🎮 Discord SDK Test Project

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)

Um projeto de exemplo completo e funcional para integração do **Discord Game SDK** com Rich Presence, demonstrando como criar uma aplicação desktop que interage com o Discord.

## ✨ Características

- ✅ **Integração completa** com Discord Game SDK
- ✅ **Rich Presence** funcional e atualizável em tempo real
- ✅ **Arquitetura modular** e bem organizada
- ✅ **Interface gráfica** estilo Discord (WinAPI)
- ✅ **Thread dedicada** para callbacks do SDK (60 FPS)
- ✅ **Código moderno** C++17 com RAII e smart pointers
- ✅ **Documentação completa** e exemplos práticos

## 📋 Pré-requisitos

- **Windows 10/11**
- **Visual Studio 2019+** (ou compilador C++17 compatível)
- **Discord Desktop** instalado e em execução
- **Discord Game SDK** (veja instalação abaixo)

## 🚀 Instalação Rápida

### 1. Clonar o Repositório

```bash
git clone https://github.com/seu-usuario/DiscordSDK_Test.git
cd DiscordSDK_Test
```

### 2. Baixar Discord Game SDK

1. Acesse: https://discord.com/developers/docs/game-sdk/sdk-starter-guide
2. Baixe o **Discord Game SDK**
3. Extraia e copie os arquivos:

```
Discord Game SDK/
├── lib/
│   └── x86_64/
│       └── discord_game_sdk.dll.lib  → External/discord-files/lib/
└── cpp/
    └── discord-files/
        └── include/                   → External/discord-files/include/
```

E copie `discord_game_sdk.dll` para a raiz do projeto.

### 3. Configurar Application ID

1. Acesse: https://discord.com/developers/applications
2. Crie uma nova aplicação ou use uma existente
3. Copie o **Application ID**
4. Edite `main.cpp` e substitua:

```cpp
const int64_t DISCORD_APPLICATION_ID = 123456789012345678; // SEU ID AQUI
```

### 4. Compilar

**Opção A: Visual Studio**
```bash
# Abra DiscordSDK_Test.sln
# Pressione CTRL + SHIFT + B
```

**Opção B: CMake**
```bash
cmake -B build
cmake --build build --config Release
```

**Opção C: Script Batch**
```bash
compilar_e_executar.bat
```

## 🎯 Como Usar

1. **Abra o Discord Desktop** (obrigatório)
2. **Execute** `bin/Release/DiscordSDK_Test.exe`
3. **Use os controles:**
   - `C` - Adiciona mensagem de chat (atualiza Rich Presence)
   - `V` - Alterna canal de voz (atualiza Rich Presence)
   - `R` - Alterna status de gravação
   - `ESC` - Fecha a aplicação

4. **Verifique o Rich Presence** no seu perfil do Discord!

## 📁 Estrutura do Projeto

```
DiscordSDK_Test/
├── src/
│   ├── discord_client.h/cpp    # Cliente Discord (SDK wrapper)
│   ├── window_handler.h/cpp    # Gerenciamento de janela Windows
│   ├── ui_renderer.h/cpp        # Renderização da interface
│   └── types.h                  # Estruturas de dados
├── External/
│   └── discord-files/           # Discord SDK (não commitado)
│       ├── include/             # Headers do SDK
│       └── lib/                 # Bibliotecas de link
├── docs/
│   ├── DIAGRAMAS.md             # Diagramas completos (12 diagramas)
│   ├── DIAGRAMAS_SIMPLIFICADOS.md  # Versões simplificadas
│   ├── DIAGRAMAS_ADICIONAIS.md     # Diagramas complementares
│   └── README_DIAGRAMAS.md      # Documentação dos diagramas
├── main.cpp                     # Ponto de entrada
├── CMakeLists.txt               # Configuração CMake
├── DiscordSDK_Test.sln          # Solução Visual Studio
└── README.md                    # Este arquivo
```

## 📊 Diagramas do Projeto

O projeto inclui **diagramas avançados e completos** para facilitar o entendimento:

- 🏗️ **Diagrama de Arquitetura** - Estrutura geral do sistema
- 📐 **Diagrama de Classes UML** - Classes e relacionamentos
- 🔄 **Diagrama de Sequência** - Fluxo de inicialização e atualizações
- 🧩 **Diagrama de Componentes** - Organização modular
- 🌊 **Diagrama de Fluxo de Dados** - Fluxo completo de execução
- 🔄 **Diagrama de Estados** - Estados da aplicação
- 🧵 **Diagrama de Threads** - Concorrência e sincronização
- 📦 **Diagrama de Dependências** - Dependências entre arquivos
- 🎯 **Diagrama de Casos de Uso** - Interações com usuário
- 🔌 **Diagrama de Integração** - Integração com Discord SDK
- 📊 **Diagrama de Ciclo de Vida** - Ciclo completo da aplicação
- 🚀 **Diagramas Adicionais** - Deployment, rede, timing, performance

📖 **Ver todos os diagramas:** [`docs/DIAGRAMAS.md`](docs/DIAGRAMAS.md)

## 🏗️ Arquitetura

O projeto utiliza uma arquitetura modular:

- **`DiscordClient`**: Encapsula toda a lógica do Discord SDK
  - Thread dedicada para callbacks
  - Gerenciamento automático de Rich Presence
  - Callbacks para eventos do usuário

- **`WindowHandler`**: Gerencia a janela Windows
  - Loop de mensagens
  - Processamento de teclado
  - Integração com DiscordClient

- **`UIRenderer`**: Renderiza a interface
  - Cores oficiais do Discord
  - Layout estilo Discord
  - Renderização via WinAPI GDI

## 📖 Exemplo de Código

```cpp
// Inicializar cliente Discord
auto discordClient = std::make_unique<DiscordClient>();
discordClient->Initialize(APPLICATION_ID);

// Atualizar Rich Presence
discordClient->UpdateRichPresence(
    "Jogando",                    // State
    "Discord SDK Test",           // Details
    "game_icon",                  // Large Image Key
    "Meu Jogo"                    // Large Image Text
);

// Callback para atualizações do usuário
discordClient->SetOnUserUpdate([](const std::string& username, const std::string& userId) {
    std::cout << "Usuário: " << username << std::endl;
});
```

## 🔧 Configuração Avançada

### Usar CMake

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Configurar Visual Studio

O projeto já está configurado, mas se precisar ajustar:

- **Include Directories**: `$(SolutionDir)External\discord-files\include`
- **Library Directories**: `$(SolutionDir)External\discord-files\lib`
- **Additional Dependencies**: `discord_game_sdk.dll.lib`

## 🐛 Troubleshooting

### Discord SDK não inicializa
- ✅ Certifique-se que o **Discord está aberto**
- ✅ Verifique se o **Application ID está correto**
- ✅ Confirme que `discord_game_sdk.dll` está no diretório de execução

### Rich Presence não aparece
- ✅ Aguarde alguns segundos após iniciar
- ✅ Pressione `C` ou `V` para forçar atualização
- ✅ Verifique se "Exibir atividade de jogo" está ativado no Discord
- ✅ Reinicie o Discord se necessário

### Erros de compilação
- ✅ Verifique se todos os arquivos do SDK foram copiados
- ✅ Confirme que está usando **C++17** ou superior
- ✅ Certifique-se que os caminhos estão corretos

## 📚 Documentação Adicional

- [Discord Game SDK Docs](https://discord.com/developers/docs/game-sdk/sdk-starter-guide)
- [Discord Developer Portal](https://discord.com/developers/applications)
- [C++17 Reference](https://en.cppreference.com/w/cpp/17)

## 🤝 Contribuindo

Contribuições são bem-vindas! Sinta-se à vontade para:

1. Fazer fork do projeto
2. Criar uma branch (`git checkout -b feature/MinhaFeature`)
3. Commit suas mudanças (`git commit -m 'Adiciona MinhaFeature'`)
4. Push para a branch (`git push origin feature/MinhaFeature`)
5. Abrir um Pull Request

## 📝 Licença

Este projeto está licenciado sob a **MIT License** - veja o arquivo [LICENSE](LICENSE) para detalhes.

## 🙏 Agradecimentos

- Discord pela excelente SDK
- Comunidade C++ por recursos e exemplos

## 📞 Suporte

Se encontrar problemas ou tiver dúvidas:

- Abra uma [Issue](https://github.com/seu-usuario/DiscordSDK_Test/issues)
- Consulte a documentação em `docs/`
- Veja os exemplos em `examples/`

---

**⭐ Se este projeto foi útil, considere dar uma estrela!**

**Criado com ❤️ para a comunidade**
