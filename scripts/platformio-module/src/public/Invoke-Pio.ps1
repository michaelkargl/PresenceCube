Function Invoke-Pio {
    [CmdletBinding()]
    Param(
        [Parameter(ValueFromRemainingArguments)]
        [string[]] $PioArguments
    )

    $PioCommand = Find-PioCommand `
        -CommandNames ('platformio', 'pio') `
        -FallbackSearchPath (Join-Path $HOME '.platformio') `
        -Debug

    Write-Debug "PWD: $PWD"
    Write-Debug "PIO: $PioCommand"

    & $PioCommand $PioArguments

    Write-Debug "Exit Code: $LASTEXITCODE"
    if ($LASTEXITCODE -ne 0) {
        Throw "[CommandUnsuccessfulError] Command $PioCommand $PioArguments failed with error code $LASTEXITCODE"
    }
}
