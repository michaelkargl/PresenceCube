<#
.SYNOPSIS
    Ensures that the pester testing framework is installed
.DESCRIPTION
    Unless already installed, an attempt is being made to
    install the pester framework module from registered sources.
#>
[CmdletBinding()]
param ()

. (Join-Path -Resolve $PSScriptRoot '/src/private/ScopingConstants.ps1')

New-Variable @private @const -Name 'ModuleName' -Value 'Pester' 

$private:Module = Get-Module -ListAvailable | Where-Object Name -eq $ModuleName
if ($Module) {
    Write-Verbose "✅ $private:ModuleName already installed"
    return
}

Write-Warning (
    "ℹ$private:ModuleName is a testing framework required for development" +
    " but could not be found in your module list. An attempt is" +
    " made to install the module from any one of the presently" +
    " registered system package sources: $(Get-PSRepository | Out-String)"
)

Install-Module $ModuleName -Force