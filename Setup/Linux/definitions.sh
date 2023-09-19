#!/usr/bin/env bash

templateFileName="dummy.txt"
outputFileName=""
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

packageNames_ProjectTemplates="Reaper Project Templates" 
packageNames_TrackTemplates="Reaper Track Templates"
packageNames_VST3="ICST Ambisonics Plugins VST3"
packageNames_AU="ICST Ambisonics Plugins AU"
packageNames_LV2="ICST Ambisonics Plugins LV2"
packageNames_Standalone=""
