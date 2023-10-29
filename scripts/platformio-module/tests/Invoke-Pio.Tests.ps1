BeforeAll {
    $ModuleRootPath = Join-Path -Resolve $PSScriptRoot '../'
    $PrivateSrcDir = Join-Path -Resolve $ModuleRootPath 'src/private'
    $PublicSrcDir = Join-Path -Resolve $ModuleRootPath 'src/public'
    
    . (Join-Path -Resolve $PrivateSrcDir 'Find-PioCommand.ps1')
    . (Join-Path -Resolve $PublicSrcDir 'Invoke-Pio.ps1')
}


Describe 'Invoke-Pio' {
    
    It 'Throws exception if returning with statuscode greater than 0' {
        # fakes a cli tool returning a status code
        Mock Find-PioCommand -MockWith {
            return [ScriptBlock] {
                if ($IsWindows) {
                    powershell -Command exit 12
                }
                else {
                    pwsh -Command exit 12
                }
            }
        }

        {
            Invoke-Pio
        } | Should -Throw "*[CommandUnsuccessfulError]*"
    }
}
