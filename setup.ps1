# Script para copiar arquivos Discord SDK e configurar o projeto

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Discord SDK Test Project - Setup Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Diretórios
$sourceSDK = "c:\Users\Administrador\Desktop\DiscordSDK For WarZ\Files\discord-files"
$targetProject = "c:\Users\Administrador\Desktop\DiscordSDK_Test"

# Verificar se a pasta source existe
if (-not (Test-Path $sourceSDK)) {
    Write-Host "❌ Erro: Pasta source não encontrada em $sourceSDK" -ForegroundColor Red
    exit 1
}

Write-Host "✓ Pasta source encontrada" -ForegroundColor Green
Write-Host ""

# Copiar headers
Write-Host "Copiando headers..." -ForegroundColor Yellow
$includeSource = "$sourceSDK\include"
$includeTarget = "$targetProject\External\discord-files\include"

if (Test-Path $includeSource) {
    Copy-Item "$includeSource\*" $includeTarget -Force -Recurse
    Write-Host "Headers copiados com sucesso" -ForegroundColor Green
}
else {
    Write-Host "Pasta include nao encontrada" -ForegroundColor Red
}

Write-Host ""

# Copiar biblioteca
Write-Host "Copiando biblioteca..." -ForegroundColor Yellow
$libSource = "$sourceSDK\lib"
$libTarget = "$targetProject\External\discord-files\lib"

if (Test-Path $libSource) {
    Copy-Item "$libSource\*" $libTarget -Force -Recurse
    Write-Host "Biblioteca copiada com sucesso" -ForegroundColor Green
}
else {
    Write-Host "Pasta lib nao encontrada" -ForegroundColor Red
}

Write-Host ""

# Copiar DLL
Write-Host "Copiando DLL..." -ForegroundColor Yellow
$dllSource = "c:\Users\Administrador\Desktop\DiscordSDK For WarZ\Files\discord_game_sdk.dll"
$dllTarget = "$targetProject\discord_game_sdk.dll"

if (Test-Path $dllSource) {
    Copy-Item $dllSource $dllTarget -Force
    Write-Host "DLL copiada com sucesso" -ForegroundColor Green
}
else {
    Write-Host "DLL nao encontrada em $dllSource" -ForegroundColor Red
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "Setup concluído!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "Próximos passos:" -ForegroundColor Cyan
Write-Host "1. Abra o projeto no Visual Studio"
Write-Host "2. Configure as propriedades do projeto:"
Write-Host "   - Linker → Input → Additional Dependencies"
Write-Host "   - Adicione: ..\External\discord-files\lib\discord_game_sdk.dll.lib"
Write-Host "3. C/C++ → Additional Include Directories"
Write-Host "   - Adicione: ..\..\External\discord-files\include"
Write-Host "4. Compile e execute!"
Write-Host ""
