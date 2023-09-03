Function Find-PioFile {
    [CmdletBinding()]
    Param (
        [Parameter(Mandatory)]
        [ValidateNotNullOrEmpty()]
        [string] $CommandName,
        # --------------------------------
        [Parameter()]
        [ValidateScript({ Test-Path $_ })]
        [string] $SearchPath
    )

    Write-Debug "Attempting to find command $CommandName in path $SearchPath";
    $CommandPath = Get-ChildItem `
        -Recurse -File `
        -LiteralPath $SearchPath `
        -Filter $CommandName | Select-Object -First 1 -ExpandProperty 'FullName';
    
    $CommandFound = [string]::IsNullOrWhiteSpace($CommandPath);
    if ( -not $CommandFound ) {
        Throw [Management.Automation.CommandNotFoundException] (
            "Unable to find $PioCommandName in $SearchPath and subfolders"
        );
    }
        
    Return $CommandPath
}

Function Find-Pio {
    [CmdletBinding()]
    Param (
        [Parameter(Mandatory)]
        [ValidateNotNullOrEmpty()]
        [string] $CommandName,
        # --------------------------------
        [Parameter()]
        [ValidateScript({ Test-Path $_ })]
        [string] $SearchPath,
        # --------------------------------
        [Parameter()]
        [ValidateScript({ Test-Path $_ })]
        [string] $FallbackSearchPath
    )

    try {
        $ManualSearch = -not [string]::IsNullOrWhiteSpace($SearchPath)
        if ($ManualSearch) {
            return Find-PioFile -CommandName $CommandName -SearchPath $SearchPath
        }
        
        Write-Debug 'Attempting to find command in shell registered paths';
        return (Get-Command $CommandName -ErrorAction Stop `
            | Select-Object -First 1 -ExpandProperty 'Path')
    }
    catch [System.Management.Automation.CommandNotFoundException] {
        Write-Warning "The command '$CommandName' could not be found within any of the configured PATHs: $env:PATH"
        if ($FallbackSearchPath) {
            Return Find-Pio -CommandName $CommandName -SearchPath $FallbackSearchPath
        }
        throw
    }
}


$PioCommand = 'pio';
$PioFallbackSearchPath = Join-Path $HOME '.platformio'

$PioCommand = Find-Pio -CommandName $PioCommand -FallbackSearchPath $PioFallbackSearchPath -Debug
& $PioCommand $args