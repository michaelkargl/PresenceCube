_target_environment='espressif32-test'

# developer obviously does testing => switch default environment
pwsh ./scripts/Set-DefaultEnv.ps1 -Environment $_target_environment

# -vvv can increase build performance (lots of console statements)
./run_pio.sh test --environment "$_target_environment" -vv $@
