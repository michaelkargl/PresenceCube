#Requires -Version 7.5
#Requires -PSEdition Core

<#
.SYNOPSIS
    Builds the given environments

.PARAMETER Environments
    An array of environments to build

.EXAMPLE
    ./Build-PioEnvironment.ps1 -Environments (,'native-dev', 'espressif32-dev')

.EXAMPLE
    ./Build-PioEnvironment.ps1 'native-dev' 'espressif32-dev'
#>
Function Build-PioEnvironment {
    Param (
        [Parameter(Mandatory)]
        [ValidateNotNullOrEmpty()]
        [string[]] $Environments,
        # --------------------------------------
        [Parameter(ValueFromRemainingArguments)]
        [string[]] $RemainingArguments

    )

    Begin {
        $Script:HeadlineColor = @{
            ForegroundColor = [ConsoleColor]::Magenta
        }               
    }

    Process {
        try {
            foreach ($Environment in $Environments) {
                Write-Host @HeadlineColor "`n--------------------------------"
                Write-Host @HeadlineColor "Building $Environment environemt"
                Write-Host @HeadlineColor "--------------------------------`n"
                if($RemainingArguments) {
                    Invoke-Pio run --environment $Environment $RemainingArguments
                } else {
                    Invoke-Pio run --environment $Environment
                }
                
            }
        }
        catch {
            Write-Error "An error occured while building the $Environment environment."
            $_ | Select-Object * | Out-String | Write-Error
            Throw
        }
    }
}