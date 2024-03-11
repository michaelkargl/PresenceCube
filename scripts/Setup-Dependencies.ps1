$ScriptName = $MyInvocation.MyCommand.Name
$ProjectRoot = Split-Path -Parent $PSScriptRoot

$SrcPath = Join-Path $ProjectRoot 'src'
$ConfigurationPath = Join-Path $SrcPath 'Kconfig.projbuild'
$LibraryPath = Join-Path $ProjectRoot 'lib'
$IdfComponentsPath = Join-Path $ProjectRoot 'components'

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
        Get-ChildItem -File -Recurse:$Recurse -LiteralPath $Path | Where-Object {
            $_ -match 'Kconfig(.projbuild)?$'
        }
    }
}

Function Join-ConfigurationFile {
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
        [string[]]$NewLines = @([System.Environment]::NewLine * 2)
        

        # TODO: Before starting any concatenation, check if the the source menu entries
        #       are already part of the target file. Otherwise you couldn't upgrade
        #       library menus without removing all the menu entries. Create a ticket for
        #       that and note that the upgrade instructions in the comment box (below)
        #       will also have to be adjusted.

        [System.IO.File]::AppendAllText($Path, @"
        

# ┌─────────────────────────────────────────────────────────────────────────────────────────┐
# │                                                                                         │
# │ │  Below you can find menu entries automatically concatenated from external sources     │
# │ │  (ie. configuration items from external libraries or idf-components). To update /     │
# │    them (after a library version upgrade for instance), remove ALL the concatenated     |
# |    menu entries manually and re-run the config prep tool again ($ScriptName).
# |    See README.                                                                          |
# │                                                                                         │
# └─────────────────────────────────────────────────────────────────────────────────────────┘

"@);
    }

    PROCESS {
        Write-Verbose "Concatenating $FullName onto $Path"

        [string[]]$RawContent = @(Get-Content -Raw $FullName)
        [System.IO.File]::AppendAllText($Path, @"
# ┌─────────────────$('─'*$FullName.Length)─┐
# │                 $(' '*$FullName.Length) │
# │ Concated from $FullName   │
# │                 $(' '*$FullName.Length) │
# └─────────────────$('─'*$FullName.Length)─┘
"@);
        [System.IO.File]::AppendAllLines($Path, $NewLines, $Utf8NoBomEncoding);
        [System.IO.File]::AppendAllLines($Path, $RawContent, $Utf8NoBomEncoding);
    }

    END {
        Write-Verbose 'Done'
    }
}

@(
    $LibraryPath,
    $IdfComponentsPath
) | Find-ConfigurationFiles -Recurse `
| Join-ConfigurationFile -Verbose -Path $ConfigurationPath