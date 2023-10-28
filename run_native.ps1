$Script:TargetEnvironment='native-dev'

# developer obviously does development => switch default environment
./scripts/Set-DefaultEnv.ps1 -Environment $TargetEnvironment

./build_env.ps1 -Environment "$TargetEnvironment"
& "./.pio/build/$TargetEnvironment/program"
