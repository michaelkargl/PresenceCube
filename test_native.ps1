<#
.SYNOPSIS
    Sets the target environment and runs native tests
.DESCRIPTION
    Different pio environments have different dependencies.
    This command switches to 'native testing' in an effort
    to make the development of tests easier by triggering a 
    pio context switch. This reinitializes code completion
    and intelisense while removing linter errors due to 
    missing dependencies.
.EXAMPLE
    ./test_native.ps1
    Run all tests
.EXAMPLE
    ./test_native.ps1 --filter '*cexception*'
    Run only tests containing the word cexception
#>

$script:ProjectRoot = $PSScriptRoot
$script:ScriptRoot = Join-Path -Resolve $ProjectRoot 'scripts/'
$script:PioModulePath = Join-Path -Resolve $ScriptRoot 'platformio-module/platformio-module.psd1'
$script:PlatformIoIniPath = Join-Path -Resolve $ProjectRoot 'platformio.ini'
# you can find these in the platformio.ini
$Script:TargetEnvironment = 'native-test'

Import-Module $PioModulePath -Force

try
{
    # developer obviously does testing => switch default environment to force the IDE to use testing intellisense
    Set-PioDefaultEnv -PlatformioIniPath $PlatformIoIniPath `
                      -Environment $TargetEnvironment `
                      -ErrorAction Stop

    # by default UNITY prints out all tests even when they are skipped => exclude these from the output
    Invoke-Pio test --environment "$TargetEnvironment" -vv @args | Select-String -NotMatch 'SKIPPED$'
}
catch
{
    throw
}