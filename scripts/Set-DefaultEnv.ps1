<#
.SYNOPSIS
  Configures platformio to the provided environment
.DESCRIPTION
  Looks for and replaces the default_envs entry in platformio.ini.
  If found, that line is updated and written back to file.
.PARAMETER Environment
  One of the available executable platforms from platformio.ini
.EXAMPLE
  Set-DefaultEnv -Environment native-dev -WhatIf
.EXAMPLE
  Set-DefaultEnv -Environment native-dev
.EXAMPLE
  Set-DefaultEnv -Environment native-dev -PlatformioIniPath /tmp/platformio.ini
#>
[CmdletBinding(SupportsShouldProcess)]
param (
    [Parameter(Mandatory)]
    [ValidateSet('native-dev', 'native-test', 'espressif32-dev')]
    [string] $Environment,
    # --------------------
    [Parameter()]
    [string] $PlatformioIniPath = "$PSScriptRoot/../platformio.ini"
)

$PlatformioIniPath = Resolve-Path -Path $PlatformioIniPath
$defaultEnvsLinePattern = 'default_envs = {0}'
$defaultEnvsLineRegex = "^$($defaultEnvsLinePattern -f '.+')$"


$PatternMatches = Select-String -Path $PlatformioIniPath -Pattern $defaultEnvsLineRegex
$LineNumber = $PatternMatches.LineNumber
if ( -not $LineNumber ) {
    Write-Error "Unable to find pattern '$defaultEnvsLineRegex' in file $iniFilePath"
    exit 1
}

 
$Lines = Get-Content $PlatformioIniPath

    $LineNumberToArrayIndexOffset = 1
    $LineIndex = $LineNumber - $LineNumberToArrayIndexOffset
    $Lines[$LineIndex] = $defaultEnvsLinePattern -f $environment

$Lines | Out-File -FilePath $PlatformioIniPath