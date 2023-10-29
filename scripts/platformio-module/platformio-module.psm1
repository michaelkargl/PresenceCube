$SrcFolder = Join-Path -Resolve $PSScriptRoot './src'

$PrivateFunctionsFolder = Join-Path -Resolve $SrcFolder 'private'
$PrivateFunctionFiles = Get-ChildItem $PrivateFunctionsFolder -File -Filter *.ps1
$PrivateFunctionFiles | ForEach-Object {
    Write-Verbose "Dot-sourcing the private script file: $($_.FullName)"
    . $_.FullName
}

$PublicFunctionsFolder = Join-Path -Resolve $SrcFolder 'public'
$PublicFunctionFiles = Get-ChildItem $PublicFunctionsFolder -File -Filter *.ps1
$PublicFunctionFiles | ForEach-Object {
    Write-Verbose "Dot-sourcing the public script file: $($_.FullName)"
    . $_.FullName
}