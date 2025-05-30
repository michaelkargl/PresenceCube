Function New-IfndefBlock {
    PARAM(
        [Parameter(Mandatory)]
        [string] $Name,
        # -------------------------
        [Parameter(Mandatory)]
        [scriptblock] $ScriptBlock,
        # -------------------------
        [Parameter()]
        [bool] $Commented = $false
    )
    if ($Commented) {
        Write-Output '/*'
    }
    Write-Output "#ifndef $Name"
    & $ScriptBlock
    Write-Output "#endif // $Name"
    if ($Commented) {
        Write-Output '*/'
    }
    Write-Output ""
}

Function New-DefineValueBlock {
    PARAM(
        [Parameter(Mandatory)]
        [string] $Name,
        # -------------------------
        [Parameter()]
        [object] $Value = $Null,
        # -------------------------
        [Parameter()]
        [bool] $Commented = $false,
        # -------------------------
        [Parameter()]
        [scriptblock] $ScriptBlock = {}
    )
    
    # this scriptblock is passed down into another function
    # with a param called ScriptBlock. Renaming is necessary
    # to prevent endless recursion
    $DefineValueBlockScriptBlock = $ScriptBlock

    New-IfndefBlock -Name $Name -Commented $Commented -ScriptBlock {
        if([string]::IsNullOrWhiteSpace($Value)) {
            Write-Output "#define $Name"
        } else {
            Write-Output "#define $Name $Value".TrimStart()
        }
        
        & $DefineValueBlockScriptBlock
    }
}

<#
.SYNOPSIS
    Converts an SdkConfig file generated by menuconfig (.config) into an includable C header file
#>
Function Convert-MenuConfigToCHeader {
    [CmdletBinding()]
    Param (
        [Parameter(Mandatory)]
        [ValidateScript({ Test-Path $_ })]
        [string] $InputFile,
        # -----------------------------------
        [Parameter(Mandatory)]
        [ValidateNotNullOrWhiteSpace()]
        [string] $OutputFile,
        # -----------------------------------
        [Parameter()]
        [string] $IncludeGuard = "SDKCONFIG_$([guid]::NewGuid().Guid.Replace('-',''))"
    )

    # The prefix menuconfig uses to prefix config values
    New-Variable -Option Constant VariablePrefix 'CONFIG_'
    # Unique string to prevent duplicate including of the same header file (manual #pragma once)
    
    & {
        Write-Output '// -----------------------------------------------------------------------'
        Write-Output '// This is generated code, do not edit unless you know what you are doing!'
        Write-Output '// -----------------------------------------------------------------------'
        Write-Output ''
    } | Tee-Object -FilePath $OutputFile

    New-DefineValueBlock -Name $IncludeGuard -ScriptBlock {
        Write-Output ''
        Write-Output '// menuconfig uses y and n macros for boolean values'
        Write-Output '// we need to assign values to them to be able to do'
        Write-Output '// #if checks'
        New-DefineValueBlock -Name 'y' -Value 1
        New-DefineValueBlock -Name 'n' -Value 0

        Get-Content $InputFile | ForEach-Object {
            $Line = $_
        
            # shell comment to C comments
            $Line = $Line -replace '^#', '//'
        
            # match anything except '=' until the first occurance of '=' and call it <symbol>
            # then match anything after the = and call it <value>
            $validAssignOperation = $Line -match '^( )*(?<comment>(//)?)( )*(?<symbol>[^=]*)=(?<value>.+)$'
            if ($validAssignOperation) {
                $Comment = $Matches.comment
                $Symbol = $Matches.symbol
                $Value = $Matches.value
                $IsCommented = -not [string]::IsNullOrWhiteSpace($Comment)
                New-DefineValueBlock `
                    -Commented $IsCommented `
                    -Name $Symbol `
                    -Value $Value
            }
            else {
                $CommentedLine = $Line -match '^( )*//'
                if ($CommentedLine) {
                    # keep commented lines intact
                    Write-Output $Line
                }
            }
        }
    } | Tee-Object -Append -FilePath $OutputFile
}