$ChannelsPerOrder = @( 1, 4, 9, 16, 25, 36 )

$EncoderVersions = 
@(
    @{ Order = 1; Input = 1; Description = "1st Order Ambisonic, 1 Channel Input" },
    @{ Order = 2; Input = 1; Description = "2nd Order Ambisonic, 1 Channel Input" }
)

$DecoderVersions = 
@(
    @{ Order = 1; Output = 32; Description = "1st Order Ambisonic, 32 Channel Output" },
    @{ Order = 2; Output = 32; Description = "2nd Order Ambisonic, 32 Channel Output" }
)

#program start
Write-Output "Generating Encoder files"
$source = '.\Encoder\AmbisonicEncoder.jucer'
foreach ($element in $EncoderVersions) 
{
    $output = $ChannelsPerOrder[$element.Order]
    createFile $source $element.Input $output $element.Order $element.Input $element.Description
}

Write-Output "Generating Decoder files"
$source = '.\Decoder\AmbisonicDecoder.jucer'
foreach ($element in $DecoderVersions) 
{
    $input = $ChannelsPerOrder[$element.Order]
    createFile $source $input $element.Output $element.Order $element.Output $element.Description
}
# program end

function createFile([string]$sourceFile, [int]$numInput, [int]$numOutput, [int]$order, [int]$audioChannelNum, [string]$description)
{
    $projectId =  'O' + $order + 'CH' + $audioChannelNum.ToString('00')
    $code = 'O' + $order + '_' + $audioChannelNum + 'CH'
    $target = $sourceFile.Replace('.jucer', "_$($code).jucer")
    Copy-Item $sourceFile $target

    Write-Output "ProjectId $($projectId)"
    Write-Output "Code $($code)"
    Write-Output "Outputs $($output)"

    # Read
    $xml = [xml](Get-Content $target)

    # Modify
    $node = $xml.JUCERPROJECT
    
    $version = $node.version
    $node.id = $projectId
    $node.pluginName = "$($node.pluginName)_$($code)"
    $node.pluginDesc = $description
    $node.pluginChannelConfigs = "{$($numInput),$($numOutput)}"
    
    $exportformats = $xml.JUCERPROJECT.EXPORTFORMATS
    foreach($format in $exportformats.ChildNodes)
    {
        $format.targetFolder = "$($format.targetFolder)/$($code)"
        foreach($configuration in $format.CONFIGURATIONS.ChildNodes)
        {
            $configuration.targetName = "$($node.pluginName)_$($version.Replace('.','_'))"
        }
    }

    # Write
    Write-Output "Writing $($target)"
    $fullTarget = Resolve-Path $target
    $xml.Save( $fullTarget)
    Write-Output "Done"
    Write-Output " "
}