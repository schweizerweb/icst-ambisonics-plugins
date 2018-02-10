$ChannelsPerOrder = @( 1, 4, 9, 16, 25, 36 )

$EncoderVersions = 
@(
    @{ Order = 1; Input = 1; Description = "1st Order Ambisonic, 1 Channel Input" },
    @{ Order = 2; Input = 1; Description = "2nd Order Ambisonic, 1 Channel Input" }
)

$source = '.\Encoder\AmbisonicEncoder.jucer'

foreach ($element in $EncoderVersions) {
    $projectId =  'O' + $element.Order + 'CH' + $element.Input.ToString('00')
    $code = 'O' + $element.Order + '_' + $element.Input + 'CH'
    $output = $ChannelsPerOrder[$element.Order]
    $target = $source.Replace('.jucer', "_$($code).jucer")
    Copy-Item $source $target

    Write-Output "ProjectId $($projectId)"
    Write-Output "Code $($code)"
    Write-Output "Outputs $($output)"

    # Read
    $xml = [xml](Get-Content $target)

    # Modify
    $node = $xml.JUCERPROJECT
    
    $version = $node.version
    $node.id = $projectId
    $node.name = $node.name
    $node.pluginName = "$($node.pluginName)_$($code)"
    $node.pluginDesc = $element.Description
    $node.pluginChannelConfigs = "{$($element.Input),$($output)}"
    
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

<#
foreach ($element in $EncoderVersions) {
    $projectId =  'O' + $element.Order + 'CH' + $element.Input.ToString('00')
    $code = 'O' + $element.Order + '_' + $element.Input + 'CH'
    $output = $ChannelsPerOrder[$element.Order]
    $source = '.\Encoder\Ambisonic Encoder.jucer'
    $target = '.\Encoder\AmbisonicEncoder_' + $code + '.jucer'

    Write-Output "ProjectId $($projectId)"
    Write-Output "Code $($code)"
    Write-Output "Outputs $($output)"

    $regex1 = "(<JUCERPROJECT.*?id="")"`
    + "(?<Id>\w*?)"`
    + "("".*?name="")"`
    + "(?<ProjectName>.*?)"`
    + "("".*?version="")"`
    + "(?<Version>[\d\.]*?)"`
    + "("".*?pluginName="")"`
    + "(?<PluginName>.*?)"`
    + "("".*?pluginDesc="")"`
    + "(?<PluginDesc>.*?)"`
    + "("".*?pluginChannelConfigs=""\{)"`
    + "(?<PluginChannelConfigs>[\d,]*?)"`
    + "(\})"`
    + "("".*?>)"

    $content = [string](Get-Content $source)

    if($content -match $regex1)
    {
        $originalProjectName = $matches.ProjectName
        $version = $matches.Version    
        $originalPluginName = $matches.PluginName
    }
    else
    {
        Write-Output "No match"
    }
    $projectName = "$($originalProjectName)"
    $pluginName = "$($originalPluginName)_$($code)"
    $description = $element.Description
    $channelConfig = "$($element.Input),$($output)"
    $replaceString = ('$1{0}$2{1}$3{2}$4{3}$5{4}$6{5}$7$8' -f $projectId, $projectName, $version, $pluginName, $description, $channelConfig)
    $content = $content -replace $regex1, $replaceString
    
    Write-Output "Original project name: $($originalProjectName)"
    Write-Output "New project name: $($projectName)"
    Write-Output "Original plugin name: $($originalPluginName)"
    Write-Output "New plugin name: $($pluginName)"
    Write-Output "Version: $($version)"
    
    Write-Output "Create $($target)"
    Set-Content -Path $target -Value $content -Encoding UTF8
}
#>


