# you can find these in the platformio.ini
_target_environment='espressif32-dev'

# developer obviously does development => switch default environment
pwsh ./scripts/Set-DefaultEnv.ps1 -Environment $_target_environment
./run_pio.sh project init --ide vscode

./run_pio.sh run --target upload \
                 --target monitor \
                 --environment "$_target_environment"