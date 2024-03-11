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
// -----------------------------------------------------------------------
// This is generated code, do not edit unless you know what you are doing!
// -----------------------------------------------------------------------

#ifndef SDKCONFIG_TEST
#define SDKCONFIG_TEST

// menuconfig uses y and n macros for boolean values
// we need to assign values to them to be able to do
// #if checks
#ifndef y
#define y 1
#endif // y

#ifndef n
#define n 0
#endif // n

// Commented Line 1
/*
#ifndef CONFIG_VAR1
#define CONFIG_VAR1 "#value_var1#"
#endif // CONFIG_VAR1
*/

/*
#ifndef CONFIG_VAR11
#define CONFIG_VAR11 "//value_var1#"
#endif // CONFIG_VAR11
*/

#ifndef CONFIG_VAR2
#define CONFIG_VAR2 "#value_value2#"
#endif // CONFIG_VAR2

#ifndef CONFIG_VAR3
#define CONFIG_VAR3 "value with = signs"
#endif // CONFIG_VAR3

#ifndef CONFIG_VAR4
#define CONFIG_VAR4 123
#endif // CONFIG_VAR4

#endif // SDKCONFIG_TEST

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
                -OutputFile $ActualHeaderFilePath `
                -IncludeGuard 'SDKCONFIG_TEST'
            
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