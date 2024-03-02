$script:ProjectRoot = $PSScriptRoot
$script:ScriptRoot = Join-Path -Resolve $ProjectRoot 'scripts/'
$script:PioModulePath = Join-Path -Resolve $ScriptRoot 'platformio-module/platformio-module.psd1'
$script:PlatformIoIniPath = Join-Path -Resolve $ProjectRoot 'platformio.ini'
# you can find these in the platformio.ini
$Script:TargetEnvironment = 'espressif32-dev'

Import-Module $PioModulePath -Force

try
{
    # TODO: migrate this into a dedicated Set-PioEnvironment function to be used externally
    # developer obviously does development => switch default environment to force the IDE to use embedded intellisense
    Set-PioDefaultEnv -PlatformioIniPath $PlatformIoIniPath `
                      -Environment $TargetEnvironment `
                      -ErrorAction Stop
    # refresh intellisense
    Invoke-Pio project init --ide vscode

    Invoke-Pio run --target upload `
                 --target monitor `
                 --environment "$TargetEnvironment"
}
catch
{
    throw
}