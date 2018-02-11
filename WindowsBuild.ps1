$devenv = """c:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/devenv.exe"""
$projucer = "./../../JUCE/Projucer"
$targetPathBase = "./bin"
$targetPathZipBase = "./packages"
$releaseVersion = [string](Get-Content "./versionInformation.txt")

$pluginTypes = @( 
    @{Name = "VST"; Extension = "*.dll"},
    @{Name = "VST3"; Extension = "*.vst3"}
)

function removePathIfExists([string]$path)
{
    if(Test-Path -Path $path)
    {
        Write-Output "Delete $($path)"
        Get-ChildItem -Path "$path\\*" -Recurse | Remove-Item -Force -Recurse
        Remove-Item $path -Force -Recurse
    }
}

# cleanup
removePathIfExists $targetPathBase

# create jucer files
$ret = Start-Process powershell "& ./JucerFilesCreator.ps1 $($releaseVersion)" -Wait -PassThru
#$ret = Start-Process powershell -Argument "./JucerFilesCreator.ps1" -PassThru -Wait -NoNewWindow
if($ret.ExitCode -ne 0)
{
    Write-Output "Error calling JucerFilesCreator: $($ret.ExitCode)"
    exit
}

# loop through jucer files
$jucerFiles = Get-ChildItem -Path ./ -Filter *_*.jucer -Recurse -ErrorAction SilentlyContinue -Force
foreach($file in $jucerFiles)
{
    Write-Output "Project: $($file.Name)"
    $path = ([System.IO.Path]::GetDirectoryName($file.FullName))
    $buildPath = "$(Join-Path $path 'Builds')/"
    
    removePathIfExists $buildPath
    
    # build project files from projucer
    Write-Output "Building jucerfile..."
    $args = "--resave $($file.FullName)"
    Start-Process $projucer -ArgumentList $args -Wait -NoNewWindow

    # find solution file
    $slnFiles = Get-ChildItem -Path $buildPath -Filter *.sln -Recurse -ErrorAction SilentlyContinue -Force
    if(@($slnFiles).length -ne 1)
    {
        Write-Output "Error: $(@($slnFiles).length) sln-Files found!"
        exit
    }
    
    # build solution
    $slnFile = @($slnFiles)[0]
    Write-Output "Building solution..."
    $args = "$($slnFile.FullName) /build ""Release"""
    $process = Start-Process $devenv -ArgumentList $args -Wait -PassThru
    if($process.ExitCode -ne 0)
    {
        Write-Output "Error building $($slnFile)"
        exit
    }
    
    # copy result to build directory
    Write-Output "Copying results..."
    foreach($pluginType in $pluginTypes)
    {
        $targetPath = Join-Path $targetPathBase $pluginType.Name
        # make sure target path exists
        if(!(Test-Path $targetPath))
        {
            Write-Output "Create Path $($targetPathZipBase)"
            New-Item -ItemType directory -Path $targetPath -Force | Out-Null
        }

        # find and copy files
        $resultFiles = Get-ChildItem -Path $buildPath -Filter $pluginType.Extension -Recurse -ErrorAction SilentlyContinue -Force
        foreach($file in @($resultFiles))
        {
            $target = (Join-Path $($targetPath) $file.Name)
            $ret = Copy-Item $file.FullName $target -PassThru
            if($ret)
            {
                Write-Output $file.Name
            }
            else
            {
                Write-Output "Error copying $($target)!"
                exit
            } 
        }
    }
    Write-Output "."
}

# zip packages
Write-Output "."
Write-Output "."
Write-Output "Build packages"
if(!(Test-Path $targetPathZipBase))
{
    Write-Output "Create Path $($targetPathZipBase)"
    New-Item -ItemType directory -Path $targetPathZipBase -Force | Out-Null
}
foreach($pluginType in $pluginTypes)
{
    $targetPath = Join-Path $targetPathBase $pluginType.Name
    $sourcesFilter = Join-Path $targetPath *.*
    $targetZip = Join-Path $targetPathZipBase "$($pluginType.Name)_$($releaseVersion).zip"
    Compress-Archive -Path $sourcesFilter -DestinationPath $targetZip -Force
    Write-Output $targetZip
}

Write-Output "Done"