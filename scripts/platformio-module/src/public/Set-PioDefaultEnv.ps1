<#
.SYNOPSIS
  Configures platformio to the provided environment
.DESCRIPTION
  Looks for and replaces the default_envs entry in platformio.ini.
  If found, that line is updated and written back to file.
.PARAMETER Environment
  One of the available executable environments defined in platformio.ini
.EXAMPLE
  Set-DefaultEnv -Environment native-dev -WhatIf
.EXAMPLE
  Set-DefaultEnv -Environment native-dev
.EXAMPLE
  Set-DefaultEnv -Environment native-dev -PlatformioIniPath /tmp/platformio.ini
#>

function Set-PioDefaultEnv
{
    [CmdletBinding(SupportsShouldProcess)]
    param (
        [Parameter(Mandatory)]
        [string] $Environment,
    # --------------------
        [Parameter(Mandatory)]
        [string] $PlatformioIniPath
    )

    $PlatformioIniPath = Resolve-Path -Path $PlatformioIniPath
    $DefaultEnvsLinePattern = 'default_envs = {0}'
    $DefaultEnvsLineRegex = "^$( $DefaultEnvsLinePattern -f '.+' )$"


    $PatternMatches = Select-String -Path $PlatformioIniPath -Pattern $DefaultEnvsLineRegex
    $LineNumber = $PatternMatches.LineNumber
    if (!$LineNumber)
    {
        Write-Error "Unable to find pattern '$DefaultEnvsLineRegex' in file $iniFilePath"
        exit 1
    }

    $Lines = Get-Content $PlatformioIniPath

    $LineNumberToArrayIndexOffset = 1
    $LineIndex = $LineNumber - $LineNumberToArrayIndexOffset
    $Lines[$LineIndex] = $DefaultEnvsLinePattern -f $environment

    $Lines | Out-File -FilePath $PlatformioIniPath
}