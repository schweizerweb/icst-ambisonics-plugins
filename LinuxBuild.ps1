$powershellCommand = "pwsh"
$projucer = "./../../JUCE/Projucer"
$buildExecutable = "make"
$buildArgumentsPre = ""
$buildArgumentsPost = ""
$projectFileExtension = "Makefile"
$platformString = "linux"
$pluginTypeStrings = @("VST3;*.so")
$buildInstallScript = "Installer_Linux.ps1"

. .\BuildScript.ps1 $powershellCommand $projucer $buildExecutable $buildArgumentsPre $buildArgumentsPost $buildInstallScript $projectFileExtension $platformString $pluginTypeStrings
