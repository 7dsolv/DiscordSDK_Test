# 🚀 Como Usar o Discord SDK Test

## ✅ Aplicação em Execução!

A aplicação deve ter aberto uma janela com interface estilo Discord.

## 🎮 Controles

### Teclas Disponíveis:
- **C** - Adiciona uma mensagem de chat e atualiza Rich Presence
- **V** - Alterna conexão com canal de voz e atualiza Rich Presence  
- **R** - Alterna status de gravação
- **ESC** - Fecha a aplicação

## ⚠️ Importante: Configurar Discord SDK

**O Discord SDK não vai funcionar até você configurar o Application ID!**

### Passo 1: Obter Application ID
1. Acesse: https://discord.com/developers/applications
2. Faça login com sua conta Discord
3. Clique em **"New Application"**
4. Dê um nome (ex: "Meu Teste SDK")
5. Clique em **"Create"**
6. Na página da aplicação, copie o **"Application ID"** (número grande)

### Passo 2: Configurar no Código
1. Abra `main.cpp` no Visual Studio
2. Encontre a linha 11:
   ```cpp
   const int64_t DISCORD_APPLICATION_ID = 123456789012345678;
   ```
3. Substitua `123456789012345678` pelo seu Application ID
4. Salve (CTRL + S)

### Passo 3: Recompilar
1. No Visual Studio, pressione **CTRL + SHIFT + B**
2. Aguarde a compilação terminar
3. Execute novamente (F5 ou `executar.bat`)

## 🎯 O Que Esperar

### Sem Application ID Configurado:
- ✅ Interface funciona normalmente
- ✅ Mensagens de chat aparecem
- ✅ Controles funcionam
- ❌ Rich Presence não aparece no Discord
- ⚠️ Mensagem no console: "APPLICATION_ID não configurado!"

### Com Application ID Configurado:
- ✅ Tudo acima +
- ✅ Rich Presence aparece no seu perfil Discord
- ✅ Status atualiza em tempo real
- ✅ Console mostra: "Discord SDK inicializado!"

## 📊 Status da Aplicação

A interface mostra:
- **Lista de servidores** (lado esquerdo)
- **Lista de canais** (meio)
- **Área de chat** (direita)
- **Barra de status** (embaixo)

## 🔍 Verificar se Discord SDK Funcionou

1. Abra o Discord
2. Vá em **Configurações** → **Atividade de Jogo**
3. Certifique-se que **"Exibir atividade de jogo como status"** está ativado
4. Execute a aplicação
5. Olhe seu perfil no Discord
6. Você deve ver o Rich Presence aparecer!

## 🐛 Problemas Comuns

### "Discord SDK não inicializado"
- ✅ Certifique-se que o Discord está **aberto**
- ✅ Verifique se o Application ID está correto
- ✅ Confirme que `discord_game_sdk.dll` está em `bin\Release\`

### Rich Presence não aparece
- ✅ Aguarde alguns segundos após iniciar
- ✅ Pressione **C** ou **V** para atualizar
- ✅ Verifique se o Discord está atualizado
- ✅ Reinicie o Discord se necessário

### Janela não abre
- ✅ Verifique se há erros no console
- ✅ Recompile o projeto (CTRL + SHIFT + B)
- ✅ Execute como administrador se necessário

## 📝 Próximos Passos

Depois de configurar o Application ID:
1. Execute a aplicação
2. Pressione **C** para ver Rich Presence mudar
3. Pressione **V** para ver outro status
4. Abra o Discord e veja seu perfil atualizar!

---

**Dica:** Mantenha o console aberto para ver as mensagens de log do Discord SDK!
