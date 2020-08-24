$powershellCommand = "powershell"
$projucer = "./../../JUCE/Projucer"
$buildExecutable = """C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"""
$buildArgumentsPre = ""
$buildArgumentsPost = "/p:Configuration=Release"
$projectFileExtension = "*.sln"
$platformString = "win_x64"
$pluginTypeStrings = @("VST3;*.vst3")
$buildInstallScript = "Installer_Win64.ps1"

. .\BuildScript.ps1 $powershellCommand $projucer $buildExecutable $buildArgumentsPre $buildArgumentsPost $buildInstallScript $projectFileExtension $platformString $pluginTypeStrings