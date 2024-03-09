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
    [ValidateSet('espressif32-dev', 'native-dev')]
    [string] $Environment,
    # ----------------------------------------------
    [Parameter(ValueFromRemainingArguments)]
    $RemainingArgs
)

$script:ProjectRoot = $PSScriptRoot
$script:ScriptRoot = Join-Path -Resolve $ProjectRoot 'scripts/'
$script:PioModulePath = Join-Path -Resolve $ScriptRoot 'platformio-module/platformio-module.psd1'
$script:KconfigFile = Join-Path -Resolve $ProjectRoot './src/Kconfig.projbuild'
$script:OutputConfigFile = Join-Path $ProjectRoot "sdkconfig.$Environment"
$script:OutputHeaderFile = Join-Path $ProjectRoot "lib/presence-cube.native/sdkconfig.h"

Import-Module $PioModulePath -Force


$NativeEnvironment = $Environment.StartsWith('native-');
if ( $NativeEnvironment ) {
    $env:KCONFIG_CONFIG = $OutputConfigFile;
    $env:MENUCONFIG_STYLE = "monochrome selection=fg:white,bg:red";
    
    menuconfig $KconfigFile

    Convert-MenuConfigToCHeader `
        -InputFile $OutputConfigFile `
        -OutputFile $OutputHeaderFile
}
else {
    # automatically merges esp-idf menuconfig entries with project config entries
    Invoke-Pio run --target menuconfig --environment $Environment
}