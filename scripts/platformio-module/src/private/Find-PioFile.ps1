<#
.SYNOPSIS
    Attempts to find the given command file on disk
.DESCRIPTION
    Given a search path, the command attempts to find it recursively
    in the given path.
.OUTPUTS
    if found, returns the absolute command path
    if not found, throws an Management.Automation.CommandNotFoundException
#>
Function Find-PioFile {
    [CmdletBinding()]
    Param (
        [Parameter(Mandatory)]
        [ValidateNotNullOrEmpty()]
        [string[]] $CommandNames,
        # --------------------------------
        [Parameter()]
        [string] $SearchPath
    )
    
    if (-not [string]::IsNullOrWhiteSpace($SearchPath) -and -not (Test-Path "$SearchPath")) {
        Throw [Management.Automation.CommandNotFoundException](
            "[Management.Automation.CommandNotFoundException] Unable to find $PioCommandName`: Search path '$SearchPath' does not exist."
        );
    }

    Foreach ($CommandName in $CommandNames) {
        Write-Debug "Attempting to find command '$CommandName' in path $SearchPath";
        $CommandPath = Get-ChildItem `
            -Recurse -File `
            -LiteralPath $SearchPath `
            -Filter $CommandName | Select-Object -First 1 -ExpandProperty 'FullName';

        $CommandFound = ![string]::IsNullOrWhiteSpace($CommandPath);
        If ($CommandFound) {
            Return $CommandPath
        }
    }

    Throw [Management.Automation.CommandNotFoundException](
        "[Management.Automation.CommandNotFoundException] Unable to find $PioCommandName in $SearchPath and subfolders"
    );
}