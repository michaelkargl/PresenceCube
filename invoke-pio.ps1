

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