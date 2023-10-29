<#
.SYNOPSIS
    Runs available pester tests for all scripts
.DESCRIPTION
    This script invokes all available pester tests fo the scripts
    in the scripts folder. Unless already installed, an attempt to
    install pester is made.
#>

$script:ScriptDirectory = $PSScriptRoot
$script:PesterInstallScript = Join-Path -Resolve $script:ScriptDirectory 'platformio-module/Install-Pester.ps1'

& $script:PesterInstallScript

Invoke-Pester -Path $script:ScriptDirectory