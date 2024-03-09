New-Variable -Name 'const' -Value (@{
    Option = [System.Management.Automation.ScopedItemOptions]::Constant
})

New-Variable -Name 'script' -Value (@{
    Scope = 'script'
})

New-Variable -Name 'private' -Value (@{
    Visibility = [System.Management.Automation.SessionStateEntryVisibility]::Private
})

New-Variable -Name 'privateConst' -Value ($private + $const)
