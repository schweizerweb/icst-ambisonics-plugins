$files = gci -recurse -Filter *.xml

if($files)
{
    write-output "convert $($files.count) files."
		
	foreach ($file in $files) 
    {
        write-output "createPreset(""$($file.BaseName)"", {"

        $xml = [xml](Get-Content $file)

        foreach($point in $xml.AmbisonicsPreset.Points.ChildNodes)
        {
            $subWoofer = If($point.FilterType -ne 0) {", true"} Else {""}
            Write-Output "`t{ $([math]::Round($point.X, 6)), $([math]::Round($point.Y, 6)), $([math]::Round($point.Z, 6)), ""$($point.Name)""$($subWoofer)},"
        }

        Write-Output "});"
        Write-Output ""
    }
}