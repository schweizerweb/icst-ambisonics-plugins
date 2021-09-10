$releaseVersion = [string](Get-Content "./versionInformation.txt")

$EncoderVersions =
@(
@{ Suffix = "_Mono"; ChannelConfig = "{1,64}"; Defines = "MULTI_ENCODER_MODE=0 JUCE_MODAL_LOOPS_PERMITTED=1"; Description = "ICST Ambisonics Encoder Mono"; PluginCode = "AECS" },
@{ Suffix = "_Multi"; ChannelConfig = "{64,64}"; Defines = "MULTI_ENCODER_MODE=1 JUCE_MODAL_LOOPS_PERMITTED=1"; Description = "ICST Ambisonics Encoder Multi"; PluginCode = "AECM" }
)

$DecoderVersions =
@(
@{ Suffix = ""; ChannelConfig = "{64,64}"; Defines = "JUCE_MODAL_LOOPS_PERMITTED=1"; Description = "ICST Ambisonics Decoder"; PluginCode = "ADCM" }
)

function createFile([string]$sourceFile, [string]$suffix, [string]$channelConfig, [string]$defines, [string]$description, [string]$pluginUniqueCode)
{
    $codeBundle = $suffix.Replace('_', '-')
    $target = $sourceFile.Replace('.jucer', "$($suffix)_AutoGen.jucer")
    Copy-Item $sourceFile $target

    Write-Output "Outputs $($output)"

    # Read
    $xml = [xml](Get-Content $target)

    # Modify
    $node = $xml.JUCERPROJECT

    if(![string]::IsNullOrEmpty($releaseVersion))
    {
        $node.version = $releaseVersion
    }

    $node.id = $pluginUniqueCode
    $node.pluginName = "$($node.pluginName)$($suffix)"
    $node.pluginDesc = $description
    $node.pluginCode = $pluginUniqueCode
	$node.bundleIdentifier = "$($node.bundleIdentifier)$(($codeBundle).ToLower())"
	$node.setAttribute("defines", "$($defines)")
	$node.setAttribute("pluginChannelConfigs", "$($channelConfig)")
    $exportformats = $xml.JUCERPROJECT.EXPORTFORMATS
    foreach($format in $exportformats.ChildNodes)
    {
        $format.targetFolder = "$($format.targetFolder)/$($pluginUniqueCode)"
        foreach($configuration in $format.CONFIGURATIONS.ChildNodes)
        {
            $configuration.SetAttribute("targetName", "$($node.pluginName)")
        }
    }

    # Write
    Write-Output "Writing $($target)"
    $fullTarget = Resolve-Path $target
    $xml.Save( $fullTarget)
    Write-Output "Done"
    Write-Output " "
}

Write-Output "Deleting Auto-Generated jucer-files"
Remove-Item -Path ./Encoder/*AutoGen.jucer -Force
Remove-Item -Path ./Decoder/*AutoGen.jucer -Force

#program start
Write-Output "Generating Encoder files"
$source = '.\Encoder\AmbisonicEncoder.jucer'
foreach ($element in $EncoderVersions) 
{
    createFile $source $element.Suffix $element.ChannelConfig $element.Defines $element.Description $element.PluginCode
}

Write-Output "Generating Decoder files"
$source = '.\Decoder\AmbisonicDecoder.jucer'
foreach ($element in $DecoderVersions) 
{
    createFile $source $element.Suffix $element.ChannelConfig $element.Defines $element.Description $element.PluginCode
}
return 0
# program end
