#Requires -Version 7.5
#Requires -PSEdition Core

Param (
    [Parameter(Mandatory, ValueFromRemainingArguments)]
    [ValidateNotNullOrEmpty()]
    [string[]] $Environments
)

$script:ProjectRoot = $PSScriptRoot
$script:ScriptRoot = Join-Path -Resolve $ProjectRoot 'scripts/'
$script:PioModulePath = Join-Path -Resolve $ScriptRoot 'platformio-module/platformio-module.psd1'

Import-Module $PioModulePath -Force

Build-PioEnvironment -Environments $Environments