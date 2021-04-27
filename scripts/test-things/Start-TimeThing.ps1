$ScriptPath = Split-Path -Parent $MyInvocation.MyCommand.Path

. $(Join-Path $ScriptPath 'Start-SleepLoudly.ps1')


<#
.SYNOPSIS
 Publishes the current time in an endless loop
.DESCRIPTION
 Beware, this is intended to provide development data only and is therefore 
 not intended to be highly precise
.EXAMPLE
 (Get-Date).ToShortTimeString() | Invoke-WebRequest -Method Post -Uri "$MainfluxUriBase/$Topic" -Headers @{ Authorization = $ThingKey }
#>
Function Start-TimeThing {
    PARAM(
        [String]$BaseUri,
        [String]$Topic,
        [String]$Token,
        [int] $TickLengthSeconds,
        [String]$DateTimeFormat = '%H:%m:%s'
    )

    $RetryInterval = 1;

    while ( $true ) {
        Try {
            $Value = Get-Date -Format $DateTimeFormat
            $Value | ConvertTo-Json `
                   | Invoke-WebRequest -Method Post `
                                      -Uri "$BaseUri/$Topic" `
                                      -Headers @{
                Authorization = $Token
            } -TimeoutSec 2 | Out-Null

            Write-Host
            Start-SleepLoudly -Seconds $TickLengthSeconds `
                              -Status "Current: $Value. Next update in..."
        } Catch {
            $RetryInterval = If ( $RetryInterval -lt 60 ) { $RetryInterval + 1 } Else { $RetryInterval }
            Write-Error "Failed to update time: $_. Retrying in $RetryInterval Seconds"
            Start-SleepLoudly -Seconds $RetryInterval -Status 'Retrying...'
        }
    }
}

. $(Join-Path $ScriptPath 'Import-EnvironmentVariables.ps1')
Start-TimeThing -BaseUri $MainfluxUriBase `
                -Topic "$ChannelTopic/time" `
                -Token $ThingKey `
                -TickLengthSeconds 10
