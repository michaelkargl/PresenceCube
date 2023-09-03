# you can find these in the platformio.ini
$Script:TargetEnvironment='espressif32-dev'

# developer obviously does development => switch default environment
./scripts/Set-DefaultEnv.ps1 -Environment $TargetEnvironment
./invoke-pio.ps1 project init --ide vscode

./invoke-pio.ps1 run --target upload `
                 --target monitor `
                 --environment "$TargetEnvironment"