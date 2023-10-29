Function Invoke-Pio {
    [CmdletBinding()]
    Param(
        [Parameter(ValueFromRemainingArguments)]
        [string[]] $PioArguments
    )

    $PioCommand = Find-PioCommand `
        -CommandName 'pio' `
        -FallbackSearchPath (Join-Path $HOME '.platformio') `
        -Debug
    
    & $PioCommand $PioArguments
    if ($LASTEXITCODE -ne 0) {
        Throw "[CommandUnsuccessfulError] Command $PioCommand $PioArguments failed with error code $LASTEXITCODE"
    }
}
