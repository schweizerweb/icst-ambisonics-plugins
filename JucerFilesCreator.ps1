$ChannelsPerOrder = @( 1, 4, 9, 16, 25, 36, 49, 64 )
$releaseVersion = [string](Get-Content "./versionInformation.txt")

$EncoderVersions =
@(
@{ Order = 7; Input = 1; Description = "7th Order Ambisonic, 1 Channel Input"; PluginCode = "AE7S" },
@{ Order = 7; Input = 64; Description = "7th Order Ambisonic, 64 Channel Input"; PluginCode = "AE7F" }
)

$DecoderVersions =
@(
@{ Order = 7; Output = 64; Description = "7th Order Ambisonic, 64 Channel Output"; PluginCode = "AD7F" }
)

function createFile([string]$sourceFile, [int]$numInput, [int]$numOutput, [int]$order, [int]$audioChannelNum, [string]$description, [string]$pluginUniqueCode)
{
    $projectId =  'o' + $order + 'ch' + $audioChannelNum.ToString('00')
    $code = 'o' + $order + '_' + $audioChannelNum + 'ch'
    $codeBundle = 'o' + $order + $audioChannelNum + 'ch'
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
    $node.pluginChannelConfigs = "{$($numInput),$($numOutput)}"
	$node.pluginCode = $pluginUniqueCode
	$node.bundleIdentifier = "$($node.bundleIdentifier)-$(($codeBundle).ToLower())"
    
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
    $output = $ChannelsPerOrder[$element.Order]
    createFile $source $element.Input $output $element.Order $element.Input $element.Description $element.PluginCode
}

Write-Output "Generating Decoder files"
$source = '.\Decoder\AmbisonicDecoder.jucer'
foreach ($element in $DecoderVersions) 
{
    $inputCount = $ChannelsPerOrder[$element.Order]
    createFile $source $inputCount $element.Output $element.Order $element.Output $element.Description $element.PluginCode
}
return 0
# program end
