$powershellCommand = "powershell"
$projucer = "./../../JUCE/Projucer"
$buildExecutable = """c:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe"""
$buildArgumentsPre = ""
$buildArgumentsPost = "/p:Configuration=Release"
$projectFileExtension = "*.sln"
$platformString = "win_x64"
$pluginTypeStrings = @("VST;*.dll", "VST3;*.vst3")

. .\BuildScript.ps1 $powershellCommand $projucer $buildExecutable $buildArgumentsPre $buildArgumentsPost $projectFileExtension $platformString $pluginTypeStrings