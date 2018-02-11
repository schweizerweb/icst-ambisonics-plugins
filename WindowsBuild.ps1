$ErrorActionPreference = "Stop"

$powershellCommand = """powershell"""
$projucer = "./../../JUCE/Projucer"
$buildExecutable = """c:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/devenv.exe"""
$buildArguments = "/build ""Release"""
$projectFileExtension = "*.sln"
$pluginTypeStrings = @("VST;*.dll", "VST3;*.vst3")

. .\BuildScript.ps1 $powershellCommand $projucer $buildExecutable $buildArguments $projectFileExtension $pluginTypeStrings