_target_environment='native-dev'

# developer obviously does development => switch default environment
pwsh ./scripts/Set-DefaultEnv.ps1 -Environment $_target_environment

if ./build_env.sh --environment "$_target_environment"
then
    # run
    ./.pio/build/$_target_environment/program
fi
