$powershellCommand = "pwsh"
$projucer = "./../../JUCE/Projucer.app/Contents/MacOS/Projucer"
$buildExecutable = "xcodebuild"
#xcodebuild -project ./Decoder/Builds/MacOSX/AmbisonicDecoder.xcodeproj -configuration Release build  
$buildArgumentsPre = "-project"
$buildArgumentsPost = "-configuration Release build"
$projectFileExtension = "*.xcodeproj"
$platformString = "osx"
$pluginTypeStrings = @("VST3;*.vst3", "AU;*.component")

. .\BuildScript.ps1 $powershellCommand $projucer $buildExecutable $buildArgumentsPre $buildArgumentsPost $projectFileExtension $platformString $pluginTypeStrings