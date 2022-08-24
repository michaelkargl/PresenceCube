_target_environment='native-test'

# developer obviously does testing => switch default environment
pwsh ./scripts/Set-DefaultEnv.ps1 -Environment $_target_environment

./run_pio.sh test --environment "$_target_environment" -vv $@
