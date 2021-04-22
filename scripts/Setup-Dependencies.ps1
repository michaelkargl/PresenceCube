$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

$SrcPath = Join-Path $ProjectRoot 'src'
$ConfigurationPath = Join-Path $SrcPath 'Kconfig.projbuild'
$LibraryPath = Join-Path $ProjectRoot 'lib'
$ConfigPath = Join-Path $ProjectRoot 'config'

If ( -not (Test-Path "$ConfigurationPath") ) {
    New-Item -ItemType File -Path $ConfigurationPath
}

Function Test-ConfigurationMenu {
    PARAM(
        [Parameter(Mandatory)]
        [String]$ConfigurationPath,
        # -------------------------
        [Parameter(Mandatory)]
        [String]$MenuName
    )

    $Pattern = 'menu "{0}"' -f $MenuName
    Select-String -Path $ConfigurationPath -Quiet -Pattern $Pattern
}


Function Find-ConfigurationFiles {
    PARAM(
        [Parameter(Mandatory, ValueFromPipeline)]
        [String]$Path,
        # --------------------
        [Parameter()]
        [Switch]$Recurse
    )

    PROCESS {
        Get-ChildItem -File -Recurse:$Recurse `
                    -Filter '*Kconfig.projbuild' `
                    -LiteralPath $Path
    }
}

Function Concatenate-ConfigurationFile {
    PARAM(
        [Parameter(Mandatory)]
        [String]$Path,
        # ------------------------------------------
        [Parameter(ValueFromPipelineByPropertyName)]
        [String]$FullName
    )

    BEGIN {
        Write-Verbose "Concatenating configuration files into $Path"

        # ESP-IDF can handle only UTF8 without BOM which powershell can't handle
        $Utf8NoBomEncoding = New-Object System.Text.UTF8Encoding $False
        [string[]]$NewLines = @([System.Environment]::NewLine*2)
    }

    PROCESS {
        Write-Verbose "Concatenating $FullName onto $Path"

        [string[]]$RawContent = @(Get-Content -Raw $FullName)
        [System.IO.File]::AppendAllLines($Path, $NewLines, $Utf8NoBomEncoding);
        [System.IO.File]::AppendAllLines($Path, $RawContent, $Utf8NoBomEncoding);
    }

    END {
        Write-Verbose 'Done'
    }
}

@(
    $LibraryPath,
    $ConfigPath
) | Find-ConfigurationFiles -Recurse `
  | Concatenate-ConfigurationFile -Verbose -Path $ConfigurationPath