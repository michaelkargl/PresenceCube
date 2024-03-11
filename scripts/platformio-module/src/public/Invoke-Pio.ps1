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
    & $PioCommand $PioArguments
    if ($LASTEXITCODE -ne 0) {
        Throw "[CommandUnsuccessfulError] Command $PioCommand $PioArguments failed with error code $LASTEXITCODE"
    }
}
