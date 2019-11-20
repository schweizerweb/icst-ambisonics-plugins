param(
    [string]$packageName
)

$dir = Split-Path ($MyInvocation.MyCommand.Path)

$tempName = "$($dir)/osx_temp.pkgproj"
Copy-Item "$($dir)/osx.pkgproj" -Destination $tempName
(Get-Content $tempName) -replace 'AmbiPluginPackageName', $packageName | Set-Content $tempName
. packagesbuild $tempName
Remove-Item $tempName

Write-Output "Build Installer Done"