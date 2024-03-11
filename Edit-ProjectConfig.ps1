<#
.SYNOPSIS
    Opens project configuration editor and prepares configuration as necessary
.DESCRIPTION
    Different environments have different configuration capabilities.
    This command knows about the needs of the specific environments and
    enables the user to configure their system.
    
.PARAMETER Environment
    This is an environment defined in the platformio.ini
.EXAMPLE 
    Edit-ProjectConfig.ps1 -environment native-dev
    Opens the config editor and prepares the config headers
.EXAMPLE
    Edit-ProjectConfig.ps1 -environment espressif32-dev
    Opens the config editor and prepares the config to be pushed to the embedded device
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
$script:DefaultsConfigFile = Join-Path $ProjectRoot "sdkconfig.defaults"
$script:OutputHeaderFile = Join-Path $ProjectRoot "lib/presence-cube.native/sdkconfig.h"
$script:OutputDefaultsHeaderFile = Join-Path $ProjectRoot "lib/presence-cube.native/sdkconfig.defaults.h"

Import-Module $PioModulePath -Force


$NativeEnvironment = $Environment.StartsWith('native-');
if ( $NativeEnvironment ) {
    $env:KCONFIG_CONFIG = $OutputConfigFile;
    $env:MENUCONFIG_STYLE = "monochrome selection=fg:black,bg:white";
    
    menuconfig $KconfigFile

    Convert-MenuConfigToCHeader `
        -InputFile $OutputConfigFile `
        -OutputFile $OutputHeaderFile
    
    Convert-MenuConfigToCHeader `
        -InputFile $DefaultsConfigFile `
        -OutputFile $OutputDefaultsHeaderFile
}
else {
    # automatically merges esp-idf menuconfig entries with project config entries
    Invoke-Pio run --target menuconfig --environment $Environment
}