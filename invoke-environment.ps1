#Requires -Version 7.5
#Requires -PSEdition Core

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
$script:PlatformIoIniPath = Join-Path -Resolve $ProjectRoot 'platformio.ini'

Import-Module $PioModulePath -Force

Set-PioDefaultEnv -PlatformioIniPath $PlatformIoIniPath `
    -Environment $Environment `
    -ErrorAction Stop

# refresh intellisense
Invoke-Pio project init --ide vscode

Build-PioEnvironment `
    -Environments $script:Environment `
    -ErrorAction Stop `
    -vv


$NativeEnvironment = $Environment.StartsWith('native-');
if ( $NativeEnvironment ) {
    & "./.pio/build/$script:Environment/program" $RemainingArgs
}
else {
    Invoke-Pio -ErrorAction Stop run `
        --target upload `
        --target monitor `
        --environment "$TargetEnvironment" `
        $RemainingArgs
}
