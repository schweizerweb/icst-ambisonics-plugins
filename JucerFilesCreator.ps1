$ChannelsPerOrder = @( 1, 4, 9, 16, 25, 36 )
$VersionFile = "./versionInformation.txt"
$releaseVersion = [string](Get-Content "./versionInformation.txt")

$EncoderVersions =
@(
@{ Order = 1; Input = 1; Description = "1st Order Ambisonic, 1 Channel Input" },
@{ Order = 2; Input = 1; Description = "2nd Order Ambisonic, 1 Channel Input" },
@{ Order = 3; Input = 1; Description = "3nd Order Ambisonic, 1 Channel Input" },
@{ Order = 4; Input = 1; Description = "4nd Order Ambisonic, 1 Channel Input" },
@{ Order = 5; Input = 1; Description = "5nd Order Ambisonic, 1 Channel Input" }
)

$DecoderVersions =
@(
@{ Order = 1; Output = 32; Description = "1st Order Ambisonic, 32 Channel Output" },
@{ Order = 2; Output = 32; Description = "2nd Order Ambisonic, 32 Channel Output" },
@{ Order = 3; Output = 32; Description = "3nd Order Ambisonic, 32 Channel Output" },
@{ Order = 4; Output = 32; Description = "4nd Order Ambisonic, 32 Channel Output" },
@{ Order = 5; Output = 32; Description = "5nd Order Ambisonic, 32 Channel Output" }
)

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

    if(![string]::IsNullOrEmpty($releaseVersion))
    {
        $node.version = $releaseVersion
    }

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
return 0
# program end