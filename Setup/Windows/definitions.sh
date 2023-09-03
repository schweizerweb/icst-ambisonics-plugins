#!/usr/bin/env bash

templateFileName="win64.iss"
outputFileName="win64_temp.iss"
packageSectionMark="; __FILE_SECTION__"
installerSectionMark="; __COMPONENT_SECTION__"
packageNameMark="AmbiPluginPackageName"
readmePathMark="AmbiPluginReadmePath"
packageVersionMark="AmbiPluginVersion"

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

packageExtraFlags_VST3="compact"