$releaseVersion = [string](Get-Content "./versionInformation.txt")

$EncoderVersions =
@(
@{ Input = 1; Description = "1st-7th Order Ambisonic, 1 Channel Input"; PluginCode = "AEXS" },
@{ Input = 64; Description = "1st-7th Order Ambisonic, 1-64 Channel Input"; PluginCode = "AEXF" }
)

$DecoderVersions =
@(
@{  Output = 64; Description = "1st-7th Order Ambisonic, 64 Channel Output"; PluginCode = "ADXF" }
)

function createFile([string]$sourceFile, [int]$numInput, [int]$numOutput, [int]$audioChannelNum, [string]$description, [string]$pluginUniqueCode)
{
    $projectId =  'ch' + $audioChannelNum.ToString('00')
    $code = $audioChannelNum.ToString() + 'ch'
    $codeBundle = $audioChannelNum.ToString() + 'ch'
    $target = $sourceFile.Replace('.jucer', "_$($code)_AutoGen.jucer")
    Copy-Item $sourceFile $target

    Write-Output "ProjectId $($projectId)"
    Write-Output "Code $($code)"
    Write-Output "Outputs $($output)"

    # Read
    $xml = [xml](Get-Content $target)

    # Modify
    $node = $xml.JUCERPROJECT

    if(![string]::IsNullOrEmpty($releaseVersion))
    {
        $node.version = $releaseVersion
    }

    $node.id = $projectId
    $node.pluginName = "$($node.pluginName)_$($code)"
    $node.pluginDesc = $description
	$node.pluginCode = $pluginUniqueCode
	$node.bundleIdentifier = "$($node.bundleIdentifier)-$(($codeBundle).ToLower())"
    $node.defines = "JUCE_MODAL_LOOPS_PERMITTED=1 MAX_NUM_INPUT=$($numInput)"
    
    $exportformats = $xml.JUCERPROJECT.EXPORTFORMATS
    foreach($format in $exportformats.ChildNodes)
    {
        $format.targetFolder = "$($format.targetFolder)/$($code)"
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
    createFile $source $element.Input 64 $element.Input $element.Description $element.PluginCode
}

Write-Output "Generating Decoder files"
$source = '.\Decoder\AmbisonicDecoder.jucer'
foreach ($element in $DecoderVersions) 
{
    createFile $source 64 $element.Output $element.Output $element.Description $element.PluginCode
}
return 0
# program end
