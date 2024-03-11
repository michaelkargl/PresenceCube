BeforeAll {
    $ModuleRootPath = Join-Path -Resolve $PSScriptRoot '../';
    $PublicSrcDir = Join-Path -Resolve $ModuleRootPath 'src/public';
    $PrivateSrcDir = Join-Path -Resolve $ModuleRootPath 'src/private';
    
    . (Join-Path -Resolve $PublicSrcDir 'Convert-MenuConfigToCHeader.ps1');
    . (Join-Path -Resolve $PrivateSrcDir 'ScopingConstants.ps1');

    New-Variable @private @script @const MenuconfigFileContent @'
# Commented Line 1
# CONFIG_VAR1="#value_var1#"
# CONFIG_VAR11="//value_var1#"

CONFIG_VAR2="#value_value2#"
CONFIG_VAR3="value with = signs"
CONFIG_VAR4=123
'@

    New-Variable @private @script @const ExpectedFileContent @'
#ifndef _SDK_CONFIG_f2a1245a4e044039b5dcc1c7d37aa471
#define _SDK_CONFIG_f2a1245a4e044039b5dcc1c7d37aa471

// Commented Line 1
// #ifndef CONFIG_VAR1
// #define CONFIG_VAR1 "#value_var1#"
// #endif

// #ifndef CONFIG_VAR11
// #define CONFIG_VAR11 "//value_var1#"
// #endif

#ifndef CONFIG_VAR2
#define CONFIG_VAR2 "#value_value2#"
#endif

#ifndef CONFIG_VAR3
#define CONFIG_VAR3 "value with = signs"
#endif

#ifndef CONFIG_VAR4
#define CONFIG_VAR4 123
#endif

#endif

'@
}

[string]$script:InputConfigFilePath;
[string]$script:ExpectedHeaderFilePath;

Describe 'Convert-MenuConfigToCHeader.ps1' {    

    BeforeEach {
        $script:InputConfigFilePath = Join-Path $TestDrive "menuconfig.$([guid]::NewGuid()).txt"
        $script:MenuconfigFileContent | Out-File -FilePath $script:InputConfigFilePath

        $script:ExpectedHeaderFilePath = Join-Path $TestDrive "header.$([guid]::NewGuid()).h"
        $script:ExpectedFileContent | Out-File -FilePath $script:ExpectedHeaderFilePath
    }

    Context 'Integration test' {

        It 'Converts menuconfig entries correctly' {
            $ActualHeaderFilePath = Join-Path $TestDrive 'header.h'
            $ExpectedContent = Get-Content $script:ExpectedHeaderFilePath

            Convert-MenuConfigToCHeader `
                -InputFile $script:InputConfigFilePath `
                -OutputFile $ActualHeaderFilePath
            
            $ActualContent = Get-Content $ActualHeaderFilePath

            # For easier troubleshooting
            Write-Host -ForegroundColor DarkRed "------------ Expected ----------"
            $ExpectedContent | ConvertTo-Json | Out-Host
            Write-Host -ForegroundColor DarkMagenta "-------------Actual-----------"
            $ActualContent | ConvertTo-Json | Out-Host
            Write-Host -ForegroundColor DarkGray "---------------------------------"
        
            $ActualContent | Should -Be $ExpectedContent
        }
    }
}