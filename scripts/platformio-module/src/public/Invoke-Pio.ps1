Function Invoke-Pio {
    [CmdletBinding()]
    Param(
        [Parameter(ValueFromRemainingArguments)]
        [string[]] $PioArguments
    )

    $PioCommand = Find-PioCommand `
        -CommandNames ('platformio', 'pio') `
        -FallbackSearchPaths @(
            # default pio install dir
            Join-Path $HOME '.platformio'
            # pipx install dir
            Join-Path $HOME '.local/bin'
        ) -Debug

    Write-Host "Command: '$PioCommand $PioArguments'"
    & $PioCommand $PioArguments

    Write-Debug "Exit Code: $LASTEXITCODE"
    if ($LASTEXITCODE -ne 0) {
        Throw "[CommandUnsuccessfulError] Command $PioCommand $PioArguments failed with error code $LASTEXITCODE"
    }
}