$script:ProjectRoot = $PSScriptRoot
$script:ScriptRoot = Join-Path -Resolve $ProjectRoot 'scripts/'
$script:PioModulePath = Join-Path -Resolve $ScriptRoot 'platformio-module/platformio-module.psd1'
$script:PlatformIoIniPath = Join-Path -Resolve $ProjectRoot 'platformio.ini'
# you can find these in the platformio.ini
$script:TargetEnvironment = 'native-dev'

Import-Module $PioModulePath -Force

# developer obviously does development => switch default environment to force the IDE to use native intellisense
Set-PioDefaultEnv `
    -PlatformioIniPath $PlatformIoIniPath `
    -Environment $script:TargetEnvironment `
    -ErrorAction Stop

# refresh intellisense
Invoke-Pio project init --ide vscode

Build-PioEnvironment `
    -Environments $script:TargetEnvironment `
    -ErrorAction Stop

& "./.pio/build/$script:TargetEnvironment/program"
