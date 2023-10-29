BeforeAll {
    $script:ModuleName = 'platformio-module'

    $ModuleRootPath = Join-Path -Resolve $PSScriptRoot '../'
    $ModuleManifestPath = Join-Path -Resolve $ModuleRootPath "$script:ModuleName.psd1"
    $PrivateSrcDir = Join-Path -Resolve $ModuleRootPath 'src/private'
    $PublicSrcDir = Join-Path -Resolve $ModuleRootPath 'src/public'

    Import-Module $ModuleManifestPath -Force
}

Describe 'platformio-module' {
    It 'should have imported module' {
        Get-Module $script:ModuleName | Should -Be $True
    }

    It 'does not expose functions from private folder' {
        $PrivateFunctions = @((Get-ChildItem -LiteralPath $PrivateSrcDir -File -Filter *.ps1).BaseName)        
        $ExportedFunctions = (Get-Module $script:ModuleName).ExportedFunctions.Keys

        foreach ($PrivateFunction in $PrivateFunctions) {
            $PrivateFunction | Should -Not -BeIn $ExportedFunctions `
                -Because 'private functions are meant for module internal use only.'
        }
    }

    It 'exposes all public functions' {
        $PublicFunctions = @((Get-ChildItem -LiteralPath $PublicSrcDir -File -Filter *.ps1).BaseName)
        $ExportedFunctions = (Get-Module $script:ModuleName).ExportedFunctions.Keys
        
        try {
            foreach ($PublicFunction in $PublicFunctions) {
                $PublicFunction | Should -BeIn $ExportedFunctions `
                    -Because 'all functions within this folder are meant to be exposed.'
            }
        }
        catch {
            Write-Error "Exported functions: $ExportedFunctions"
            Write-Error "Public functions: $PublicFunctions"        
            throw
        }

    }
}