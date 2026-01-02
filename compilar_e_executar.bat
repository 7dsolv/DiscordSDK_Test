@echo off
echo ========================================
echo Discord SDK Test - Compilar e Executar
echo ========================================
echo.

cd /d "%~dp0"

echo [INFO] Verificando se precisa recompilar...
echo.

REM Verificar se o Visual Studio esta disponivel
where msbuild >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [AVISO] MSBuild nao encontrado no PATH.
    echo [INFO] Por favor, compile manualmente:
    echo   1. Abra DiscordSDK_Test.sln no Visual Studio
    echo   2. Pressione CTRL + SHIFT + B para compilar
    echo   3. Execute executar.bat
    echo.
    pause
    exit /b
)

echo [INFO] Compilando projeto...
echo.

REM Tentar encontrar MSBuild
set MSBUILD_PATH=
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe" (
    set MSBUILD_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe
)

if "%MSBUILD_PATH%"=="" (
    echo [ERRO] MSBuild nao encontrado!
    echo [INFO] Por favor, compile manualmente no Visual Studio
    echo.
    pause
    exit /b
)

"%MSBUILD_PATH%" DiscordSDK_Test.sln /p:Configuration=Release /p:Platform=Win32 /t:Build /v:minimal /nologo

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Compilacao concluida com sucesso!
    echo [INFO] Executando aplicacao...
    echo.
    timeout /t 1 /nobreak >nul
    cd bin\Release
    start DiscordSDK_Test.exe
    cd ..\..
    echo.
    echo [INFO] Aplicacao iniciada!
) else (
    echo.
    echo [ERRO] Falha na compilacao!
    echo [INFO] Verifique os erros acima e tente novamente.
    echo.
    pause
    exit /b
)
