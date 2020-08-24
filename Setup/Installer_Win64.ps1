param(
    [string]$packageName,
    [string]$packageVersion
)

$innocompiler = 'C:\Program Files (x86)\Inno Setup 6\iscc.exe'

$dir = Split-Path ($MyInvocation.MyCommand.Path)

$tempName = "$($dir)/win64_temp.iss"
Copy-Item "$($dir)/win64.iss" -Destination $tempName
(Get-Content $tempName) -replace 'AmbiPluginPackageName', $packageName | Set-Content $tempName
(Get-Content $tempName) -replace 'AmbiPluginVersion', $packageVersion | Set-Content $tempName
. $innocompiler $tempName
Remove-Item $tempName

Write-Output "Build Installer Done"