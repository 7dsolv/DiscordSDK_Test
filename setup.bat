@echo off
REM Script para copiar arquivos Discord SDK

echo ========================================
echo Discord SDK Test Project - Setup
echo ========================================
echo.

setlocal enabledelayedexpansion

set "SOURCE_SDK=c:\Users\Administrador\Desktop\DiscordSDK For WarZ\Files\discord-files"
set "TARGET_PROJECT=c:\Users\Administrador\Desktop\DiscordSDK_Test"

REM Verificar se source existe
if not exist "%SOURCE_SDK%" (
    echo Erro: Pasta source nao encontrada!
    exit /b 1
)

echo [INFO] Copiando headers...
xcopy "%SOURCE_SDK%\include\*" "%TARGET_PROJECT%\External\discord-files\include\" /Y /E /I
if !ERRORLEVEL! equ 0 (
    echo [OK] Headers copiados
) else (
    echo [ERRO] Falha ao copiar headers
)

echo.
echo [INFO] Copiando biblioteca...
xcopy "%SOURCE_SDK%\lib\*" "%TARGET_PROJECT%\External\discord-files\lib\" /Y /E /I
if !ERRORLEVEL! equ 0 (
    echo [OK] Biblioteca copiada
) else (
    echo [ERRO] Falha ao copiar biblioteca
)

echo.
echo [INFO] Copiando DLL...
copy "c:\Users\Administrador\Desktop\DiscordSDK For WarZ\Files\discord_game_sdk.dll" "%TARGET_PROJECT%\discord_game_sdk.dll" /Y
if !ERRORLEVEL! equ 0 (
    echo [OK] DLL copiada
) else (
    echo [ERRO] Falha ao copiar DLL
)

echo.
echo ========================================
echo Setup concluido!
echo ========================================
echo.
echo Proximos passos:
echo 1. Abra o projeto no Visual Studio
echo 2. Configure as propriedades do projeto
echo 3. Compile e execute!
echo.
pause
