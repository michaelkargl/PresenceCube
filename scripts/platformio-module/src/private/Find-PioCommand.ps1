<#
.SYNOPSIS
    Tries to find the given command either in global space
    or in custom search directories
.DESCRIPTION
    The command name is first being searched in globa space ($PATH).
    Unless it is found, the search paths are worked through for a
    file with that particular name.
.PARAMETER CommandName
    The name of the command to be found
.PARAMETER SearchPath
    This path is searched recursively for a file with the name of the command
.PARAMETER FallbackSearchPaths
    Last resort before breaking off search. In case none
    of the options are fruitful, these paths are used as a search base.
.OUTPUTS
    If found, the absolute path to the command in question
    if it is not found, a System.Management.Automation.CommandNotFoundException
    is thrown
#>
Function Find-PioCommand {
    [CmdletBinding()]
    Param (
        [Parameter(Mandatory)]
        [ValidateNotNullOrEmpty()]
        [string[]] $CommandNames,
        # --------------------------------
        [Parameter()]
        [string] $SearchPath,
        # --------------------------------
        [Parameter()]
        [string[]] $FallbackSearchPaths = @()
    )

    try {
        $Command = $CommandNames | ForEach-Object {
            Write-Debug "Attempting to find command '$_' in PATH"
            Get-Command $_ -ErrorAction SilentlyContinue 
        } | Select-Object -First 1 -ExpandProperty Path

        if ($Command) {
            Write-Debug "Command registered: $Command"
            return $Command
        }
        else {
            Write-Warning "Commands $($CommandNames -join ', ') not registered"
        }


        $FileSearchRequested = -not [string]::IsNullOrWhiteSpace($SearchPath)
        if ($FileSearchRequested) {
            Write-Debug "Attempting to find command '$_' in $SearchPath"
            $Command = Find-PioFile -CommandNames $CommandNames -SearchPath $SearchPath
            if ( $Command ) {
                Write-Debug "Command found: $Command"
                return $Command
            } {
                Write-Warning "Command not found"
            }
        }

        throw [System.Management.Automation.CommandNotFoundException] (
            "[Management.Automation.CommandNotFoundException] Unable to find command '$CommandName'"
        )
    }
    catch [System.Management.Automation.CommandNotFoundException] {
        $FallbackSearchPaths | ForEach-Object {
            $Path = $_
            Write-Debug "Attempting to find command '$_' in $Path"
            $Command = Find-PioCommand -CommandNames $CommandNames -SearchPath $Path
            if($Command) {
                return $Command
            }
        }
    }
}