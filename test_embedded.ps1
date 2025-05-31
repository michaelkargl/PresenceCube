#Requires -Version 7.5
#Requires -PSEdition Core

<#
.SYNOPSIS
    Sets the target environment and runs embedded tests
.DESCRIPTION
    Different pio environments have different dependencies.
    This command switches to 'native testing' in an effort
    to make the development of tests easier by triggering a 
    pio context switch. This reinitializes code completion
    and intelisense while removing linter errors due to 
    missing dependencies.
.EXAMPLE
    ./test_embedded.ps1
    Run all tests
.EXAMPLE
    ./test_embedded.ps1 --filter '*cexception*'
    Run only tests containing the word cexception
#>

Write-Warning ''
Write-Warning '-------------------------------'
Write-Warning 'This function is [Obsolete]!'
Write-Warning 'Use invoke-tests directly!'
Write-Warning 'Get-Help ./invoke-tests.ps1'
Write-Warning '-------------------------------'
Write-Warning ''
Write-Host ''

$InvokeTestsScript = Join-Path $PSScriptRoot 'invoke-tests.ps1'
& $InvokeTestsScript -Environment 'espressif32-test' $args