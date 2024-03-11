<#
.SYNOPSIS
    Runs available pester tests for all scripts
.DESCRIPTION
    This script invokes all available pester tests fo the scripts
    in the scripts folder. Unless already installed, an attempt to
    install pester is made.
#>

$ScriptDirectory = $PSScriptRoot
$PesterInstallScript = Join-Path -Resolve $ScriptDirectory 'platformio-module/Install-Pester.ps1'

# TODO: make pester a dependency of the module
& $PesterInstallScript

# TLDR: calling pester tests in an isolated session shields the current powershell environment
#       from unintentional global scope pollution / mutation
# By default, tests are not run within an isolated session therefore mutations have direct effect
# on the environment and consequently all subsequent calls. This already effected the platformio 
# environment enough to fail UNITY test compilation.
pwsh -NonInteractive -CommandWithArgs @'
    $ScriptDirectory = $args[0]
    Invoke-Pester -Path $ScriptDirectory
'@ $ScriptDirectory

if ($LASTEXITCODE -ne 0) {
    Throw "$LASTEXITCODE tests are failing"
}