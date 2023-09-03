Param (
    [Parameter(Mandatory)]
    [ValidateNotNullOrEmpty()]
    [string] $Environment
)

./invoke-pio.ps1 run --environment "$environment"