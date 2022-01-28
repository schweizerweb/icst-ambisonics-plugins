$powershellCommand = "pwsh"
$projucer = "./../../JUCE/Projucer.app/Contents/MacOS/Projucer"
$buildExecutable = "xcodebuild"
#xcodebuild -project ./Decoder/Builds/MacOSX/AmbisonicDecoder.xcodeproj -configuration Release build  
$buildArgumentsPre = "-project"
$buildArgumentsPost = "-arch x86_64 -only_active_arch=no -configuration Release build"
$projectFileExtension = "*.xcodeproj"
$platformString = "osx-x64"
$pluginTypeStrings = @("VST3;*.vst3", "AU;*.component")
$buildInstallScript = "Installer_OSX.ps1"

. .\BuildScript.ps1 $powershellCommand $projucer $buildExecutable $buildArgumentsPre $buildArgumentsPost $buildInstallScript $projectFileExtension $platformString $pluginTypeStrings