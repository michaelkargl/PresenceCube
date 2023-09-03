<#
.SYNOPSIS
    Sets the target environment and runs native tests
.DESCRIPTION
    Different pio environments have different dependencies.
    This command switches to 'native testing' in an effort
    to make the development of tests easier by triggering a 
    pio context switch. This reinitializes code completion
    and intelisense while removing linter errors due to 
    missing dependencies.
.EXAMPLE
    ./test_native.ps1
    Run all tests
.EXAMPLE
    ./test_native.ps1 --filter '*cexception*'
    Run only tests containing the word cexception
#>

$Script:TargetEnvironment='native-test'

# developer obviously does testing => switch default environment
./scripts/Set-DefaultEnv.ps1 -Environment $TargetEnvironment

# by default UNITY prints out all tests even when they are skipped => exclude these from the output
./invoke-pio.ps1 test --environment "$TargetEnvironment" -vv @args | Select-String -NotMatch 'SKIPPED$'