# TODO #156: Test and document
<#
.OUTPUTS
    if found, returns the absolute command path
    if not found, throws an Management.Automation.CommandNotFoundException
#>
Function Find-PioFile
{
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
    if (!$CommandFound)
    {
        Throw [Management.Automation.CommandNotFoundException](
        "Unable to find $PioCommandName in $SearchPath and subfolders"
        );
    }

    Return $CommandPath
}

# TODO #156: Test and document
Function Find-Pio
{
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
        [string] $FallbackSearchPath
    )

    try
    {
        Write-Debug "Attempting to find path $CommandName in registered paths"
        $Command = Get-Command $CommandName -ErrorAction Stop | Select-Object -First 1 -ExpandProperty 'Path'
        if ($Command)
        {
            Write-Debug "Command registered: $Command"
            return $Command
        }
        else
        {
            Write-Warning "Command not registered"
        }


        $FileSearchRequested = -not [string]::IsNullOrWhiteSpace($SearchPath)
        if ($FileSearchRequested)
        {
            return Find-PioFile -CommandName $CommandName -SearchPath $SearchPath
        }
    }
    catch [System.Management.Automation.CommandNotFoundException]
    {
        if ($FallbackSearchPath)
        {
            Return Find-Pio -CommandName $CommandName -SearchPath $FallbackSearchPath
        }
        throw
    }
}

# TODO #156: Migrate to well tested powershell module

$PioCommand = 'pio';
$PioFallbackSearchPath = Join-Path $HOME '.platformio'
$PioCommand = Find-Pio -CommandName $PioCommand -FallbackSearchPath $PioFallbackSearchPath -Debug

& $PioCommand $args
if ($LASTEXITCODE -ne 0)
{
    # TODO #156: Test
    Throw "Command $PioCommand failed with error code $LASTEXITCODE"
}