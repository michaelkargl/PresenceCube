BeforeAll {
    $ModuleRootPath = Join-Path -Resolve $PSScriptRoot '../'
    $PrivateSrcDir = Join-Path -Resolve $ModuleRootPath 'src/private'
    
    . (Join-Path -Resolve $PrivateSrcDir 'Find-PioFile.ps1')
    . (Join-Path -Resolve $PrivateSrcDir 'Find-PioCommand.ps1')
}

Describe 'Find-PioCommand' {
    [System.IO.FileInfo] $DummyCommand;

    BeforeEach {
        $DummyCommand = New-Item -ItemType File 'TestDrive:/dummy_command' -Force

        Mock -CommandName Find-PioFile -MockWith {
            Write-Output $DummyCommand.FullName
        }
    }

    It 'Returns command path if found registered in global space' {
        $env:PATH = "$env:PATH:$($DummyCommand.DirectoryName)"
        Find-PioCommand -CommandName $DummyCommand.Name | Should -BeExactly $DummyCommand.FullName
    }

    it 'Throws exception if command can not be found and no search path is provided' {
        $UnknownCommand = [Guid]::NewGuid().Guid
        {
            Find-PioCommand -CommandName $UnknownCommand
        } | Should -Throw "*[Management.Automation.CommandNotFoundException]*"
    }

    it 'Returns command path if found in search path' {
        Find-PioCommand `
            -CommandName $DummyCommand.Name `
            -SearchPath $DummyCommand.DirectoryName `
        | Should -BeExactly $DummyCommand.FullName
    }
   
    it 'Throws exception if not found in search path and no fallback is provided' {
        $ExpectedErrorMessage = "Test Exception"
        Mock -CommandName Find-PioFile -MockWith {
            Throw $ExpectedErrorMessage
        }

        {
            Find-PioCommand -CommandName 'dummy' -SearchPath 'TestDrive:/'
        } | Should -Throw $ExpectedErrorMessage
    }

    it 'Returns command path if command found in fallback search directory' {
        # Important: Since we are using ParameterFilters here, ensure that none
        #            of these variables overlap with any parameter names, otherwise
        #            you will run into weird behaviors due to scoping issues
        $NormalSearchDir = New-Item -ItemType Directory 'TestDrive:/normal_search_path' -Force
        $FallbackSearchDir = New-Item -ItemType Directory 'TestDrive:/fallback_search_path' -Force    
        $NormalSearchTestPath = $NormalSearchDir.FullName
        $FallbackSearchTestPath = $FallbackSearchDir.FullName
        $ExpectedCommandPath = "$($FallbackSearchTestPath)/expected_command"

        Mock -CommandName Find-PioFile -MockWith {
            Throw [System.Management.Automation.CommandNotFoundException] (
                "[Exception] Throwing until the fallback case is entered"
            )
        }
        
        Mock -CommandName Find-PioFile -MockWith {
            Write-Output $ExpectedCommandPath
        } -ParameterFilter {
            $SearchPath -eq $FallbackSearchTestPath
        } -Verifiable

        Write-Host "Fallback: $FallbackSearchTestPath"
        Find-PioCommand -CommandName 'dummy' `
                        -SearchPath $NormalSearchTestPath `
                        -FallbackSearchPath $FallbackSearchTestPath `
                        | Should -BeExactly $ExpectedCommandPath
    }
}