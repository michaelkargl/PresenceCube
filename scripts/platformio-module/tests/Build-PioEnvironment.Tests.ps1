BeforeAll {
    $ModuleRootPath = Join-Path -Resolve $PSScriptRoot '../'
    $PublicSrcDir = Join-Path -Resolve $ModuleRootPath 'src/public'
    
    . (Join-Path -Resolve $PublicSrcDir 'Invoke-Pio.ps1')
    . (Join-Path -Resolve $PublicSrcDir 'Build-PioEnvironment.ps1')
}


Describe 'Build-PioEnvironment' {
    BeforeAll {
        Mock -CommandName Invoke-Pio -MockWith {
            Write-Host 'Mocked Invoke-Pio instance called'
        } -Verifiable
    }

    It "Given <ExpectedCallCount> environments, <ExpectedCallCount> builds are triggered" -ForEach @(
        @{ ExpectedCallCount = 1},
        @{ ExpectedCallCount = 2}
    ) {
        $EnvironmentsToBuild = 1..$ExpectedCallCount | ForEach-Object { "environment_$_" }
        Build-PioEnvironment -Environments $EnvironmentsToBuild
        Should -Invoke -CommandName Invoke-Pio -Times $ExpectedCallCount
    }
}
