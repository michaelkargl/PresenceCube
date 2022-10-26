_target_environment='espressif32-test'

# developer obviously does testing => switch default environment
pwsh ./scripts/Set-DefaultEnv.ps1 -Environment $_target_environment

# -vvv can increase build performance (lots of console statements)
# by default UNITY prints out all tests but skips the ones not matching a certain environment => ignore those
./run_pio.sh test --environment "$_target_environment" -vv $@ | grep --no-ignore-case --invert-match 'SKIPPED'
