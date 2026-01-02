@echo off
echo ========================================
echo Discord SDK Test - Executando...
echo ========================================
echo.

cd /d "%~dp0"

REM Verificar se o executavel existe
if exist "bin\Release\DiscordSDK_Test.exe" (
    echo [INFO] Executavel encontrado!
    echo [INFO] Iniciando aplicacao...
    echo.
    echo NOTA: Se o Discord SDK nao inicializar, edite main.cpp
    echo e configure o DISCORD_APPLICATION_ID
    echo.
    pause
    cd bin\Release
    start DiscordSDK_Test.exe
) else (
    echo [ERRO] Executavel nao encontrado!
    echo [INFO] Por favor, compile o projeto primeiro:
    echo   1. Abra DiscordSDK_Test.sln no Visual Studio
    echo   2. Pressione CTRL + SHIFT + B para compilar
    echo   3. Execute este script novamente
    echo.
    pause
)
