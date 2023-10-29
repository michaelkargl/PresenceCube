BeforeAll {
    $ModuleRootPath = Join-Path -Resolve $PSScriptRoot '../'
    $PrivateSrcDir = Join-Path -Resolve $ModuleRootPath 'src/private'

    . (Join-Path -Resolve $PrivateSrcDir 'Find-PioFile.ps1')
}

Describe 'Find-PioFile' {
    It 'Throws validation error when passing invalid search path' {
        {
            $InvalidSearchPath = [Guid]::NewGuid().Guid
            Find-PioFile -CommandName 'Get-ChildItem' -SearchPath $InvalidSearchPath
        } | Should -Throw "*Cannot validate argument on parameter 'SearchPath'*"
    }

    it 'Throws exception if command can not be found' {
        {
            $UnknownCommand = [Guid]::NewGuid().Guid
            Find-PioFile -CommandName $UnknownCommand -SearchPath TestDrive:/
        } | Should -Throw "*[Management.Automation.CommandNotFoundException]*"
    }

    it 'returns command path if found deep within nested search path' {
        $CommandName = [guid]::NewGuid().Guid;
        $SearchPath = 'TestDrive:'
        $CommandDir = New-Item -ItemType Directory (
            Join-Path $SearchPath '/a/a1/a2'
        )
        $CommandFile = New-Item -ItemType File -Path (
            Join-Path $CommandDir.FullName $CommandName
        )

        $ActualCommandPath = Find-PioFile -CommandName $CommandName -SearchPath $SearchPath
        $ActualCommandPath | Should -BeExactly $CommandFile.FullName
    }
}