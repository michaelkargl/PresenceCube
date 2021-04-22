
Function Start-SleepLoudly {
    PARAM(
        [Int] $Seconds,
        [String] $Status
    )

    $Seconds..0 | ForEach-Object {
        $Status = If ( -not $Status ) { $_ } Else { $Status }
        Write-Progress -Activity 'Sleeping...' -Status $Status -PercentComplete $(($_/$Seconds)*100)
        Start-Sleep -Seconds 1
    }
}
