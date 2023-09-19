#!/usr/bin/env bash

templateFileName="osx.pkgproj"
outputFileName="osx_temp.pkgproj"
packageSectionMark="<!--PackagesSection-->"
installerSectionMark="<!--InstallerSection-->"
fileListMark="<!--FileList-->"
packageNameMark="AmbiPluginPackageName"
readmePathMark="AmbiPluginReadmePath"
packageVersionMark="__PACKAGE_VERSION__"

arrayGet() {
    local array=$1 index=$2
    local i="${array}_$index"
    echo "${!i}"
}

packageUuids_ProjectTemplates="3E417A8E-9AF4-4B5B-8BE4-2FABA488E44E"
packageUuids_TrackTemplates="653CA616-D24D-4A5F-8A9B-20D6E48B1CFC"
packageUuids_VST3="E3F4C3A4-84CE-491C-B235-90DFADA4741A"
packageUuids_AU="9F8AB4B0-7F5D-4C16-88C0-71977FD91D95"
packageUuids_LV2="40ed8d8e-30a5-4106-af10-aa7d43293ad2"
packageUuids_Standalone=""

installerUuids_ProjectTemplates="15D1C957-8769-4050-8826-8FFC0E870392" 
installerUuids_TrackTemplates="558FABB5-B75F-45D6-A326-EE9317A2D619"
installerUuids_VST3="814B9FC0-6FF5-4ABD-8150-4C043288AF4B"
installerUuids_AU="9472FCDE-7D68-4D15-92BC-614BBB51FE09"
installerUuids_LV2="ef411ab1-dfae-4f16-935e-5807f5cce770"
installerUuids_Standalone=""

packageNames_ProjectTemplates="Reaper Project Templates" 
packageNames_TrackTemplates="Reaper Track Templates"
packageNames_VST3="ICST Ambisonics Plugins VST3"
packageNames_AU="ICST Ambisonics Plugins AU"
packageNames_LV2="ICST Ambisonics Plugins LV2"
packageNames_Standalone=""

packageTargetFolders_AU="Components"
