#Requires -Version 7.5
#Requires -PSEdition Core

Write-Warning ''
Write-Warning '-------------------------------'
Write-Warning 'This function is [Obsolete]!'
Write-Warning 'Use invoke-environment directly!'
Write-Warning 'Get-Help ./invoke-environment.ps1'
Write-Warning '-------------------------------'
Write-Warning ''
Write-Host ''

$InvokeTestsScript = Join-Path $PSScriptRoot 'invoke-environment.ps1'
& $InvokeTestsScript -Environment 'espressif32-dev'