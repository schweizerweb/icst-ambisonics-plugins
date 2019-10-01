param(
    [string]$powershellCommand,
    [string]$projucer,
    [string]$buildExecutable,
    [string]$buildArgumentsPre,
    [string]$buildArgumentsPost,
    [string]$projectFileExtension,
	[string]$platformString,
    [string[]]$pluginTypeStrings
)

$ErrorActionPreference = "Stop"
$targetPathBase = "./bin"
$targetPathZipBase = "./packages"
$releaseVersion = [string](Get-Content "./versionInformation.txt")

$pluginTypes = New-Object System.Collections.ArrayList
foreach($t in $pluginTypeStrings)
{ 
    $split = $t.Split(';')
    $temp = New-Object System.Object
    $temp | Add-Member -MemberType NoteProperty -Name "Name" -Value $split[0]
    $temp | Add-Member -MemberType NoteProperty -Name "Extension" -Value $split[1]
    $pluginTypes.Add($temp) | Out-Null
}

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
$ret = Start-Process $powershellCommand "./JucerFilesCreator.ps1" -Wait -PassThru
if($ret.ExitCode -ne 0)
{
    Write-Output "Error calling JucerFilesCreator: $($ret.ExitCode)"
    exit
}

# loop through jucer files
$jucerFiles = Get-ChildItem -Path ./ -Filter *_AutoGen.jucer -Recurse -ErrorAction SilentlyContinue -Force
foreach($file in $jucerFiles)
{
	$fileToRemove = $file.FullName
    Write-Output "Project: $($file.Name)"
    $path = ([System.IO.Path]::GetDirectoryName($file.FullName))
    $buildPath = "$(Join-Path $path 'Builds')/"
    
    removePathIfExists $buildPath
    
    # build project files from projucer
    Write-Output "Building jucerfile..."
    $args = "--resave $($file.FullName)"
    Start-Process $projucer -ArgumentList $args -Wait -NoNewWindow

    # find solution file
    $projectFiles = Get-ChildItem -Path $buildPath -Filter $projectFileExtension -Recurse -ErrorAction SilentlyContinue -Force
    if(@($projectFiles).length -ne 1)
    {
        Write-Output "Error: $(@($projectFiles).length) sln-Files found!"
        exit
    }
    
    # build solution
    $projectFile = @($projectFiles)[0]
    Write-Output "Building solution..."
    $process = Start-Process $buildExecutable "$($buildArgumentsPre) $($projectFiles.FullName) $($buildArgumentsPost)" -Wait -PassThru
    if($process.ExitCode -ne 0)
    {
        Write-Output "Error building $($projectFile)"
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
            $ret = Copy-Item $file.FullName $target -Recurse -PassThru
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
	Remove-Item $fileToRemove -Force
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
    $targetZip = Join-Path $targetPathZipBase "ICST_AmbiPlugins_$($pluginType.Name)_$($releaseVersion)_$($platformString).zip"
	if(Test-Path $targetZip)
	{
		Remove-Item $targetZip -Force
	}
	
    Compress-Archive -Path $sourcesFilter -DestinationPath $targetZip -Force
    Write-Output $targetZip
}

Write-Output "Done"