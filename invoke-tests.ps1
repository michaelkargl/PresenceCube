#Requires -Version 7.5
#Requires -PSEdition Core

<#
.SYNOPSIS
    Runs all tests for a given environment
.DESCRIPTION
    Different pio environments have different dependencies.
    This command switches to 'native testing' in an effort
    to make the development of tests easier by triggering a 
    pio context switch. This reinitializes code completion
    and intelisense while removing linter errors due to 
    missing dependencies.
.PARAMETER Environment
    This is an environment defined in the platformio.ini
.EXAMPLE
    invoke-tests -Environment native-test
    Run all tests
.EXAMPLE
    invoke-tests -Environment native-test --filter '*cexception*'
    Run only tests containing the word cexception
#>
PARAM(
    [Parameter(Mandatory)]
    [ValidateSet('espressif32-test', 'native-test')]
    [string] $Environment,
    # ----------------------------------------------
    [Parameter(ValueFromRemainingArguments)]
    $RemainingArgs
)

$script:ProjectRoot = $PSScriptRoot
$script:ScriptRoot = Join-Path -Resolve $ProjectRoot 'scripts/'
$script:PioModulePath = Join-Path -Resolve $ScriptRoot 'platformio-module/platformio-module.psd1'
$script:PlatformIoIniPath = Join-Path -Resolve $ProjectRoot 'platformio.ini'
$script:InvokeScriptTestsScript = Join-Path $ScriptRoot 'Invoke-ScriptTests.ps1'

Import-Module $PioModulePath -Force

# developer obviously does testing => switch default environment to force the IDE to use testing intellisense
Set-PioDefaultEnv `
    -PlatformioIniPath $PlatformIoIniPath `
    -Environment $Environment `
    -ErrorAction Stop

& $InvokeScriptTestsScript

# Careful with output verbosity!
# -vvv can decrease build performance drastically as it issues a lot of console statements
Invoke-Pio test --environment "$Environment" -vv @RemainingArgs