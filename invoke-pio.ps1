#Requires -Version 7.5
#Requires -PSEdition Core

$ScriptDirPath = Join-Path $PSScriptRoot 'scripts'
$PlatformIoModule = Join-Path $ScriptDirPath 'platformio-module/platformio-module.psd1'

Import-Module $PlatformIoModule -Force
Invoke-Pio $args