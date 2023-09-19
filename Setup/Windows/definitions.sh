#!/usr/bin/env bash

templateFileName="win64.iss"
outputFileName="win64_temp.iss"
packageSectionMark="; __FILE_SECTION__"
installerSectionMark="; __COMPONENT_SECTION__"
dirSectionMark="; __DIR_SECTION__"
packageNameMark="AmbiPluginPackageName"
readmePathMark="AmbiPluginReadmePath"
packageVersionMark="AmbiPluginVersion"
redistMark="__REDIST_PLACEHOLDER__"

arrayGet() {
    local array=$1 index=$2
    local i="${array}_$index"
    echo "${!i}"
}


packageNames_ProjectTemplates="Reaper Project Templates" 
packageNames_TrackTemplates="Reaper Track Templates"
packageNames_VST3="ICST Ambisonics Plugins VST3"
packageNames_LV2="ICST Ambisonics Plugins LV2"
packageNames_Standalone=""

packageExtraFlags_VST3="compact"

packageTargetBaseFolder_ProjectTemplates="{userappdata}/REAPER"
packageTargetBaseFolder_TrackTemplates="{userappdata}/REAPER"
packageTargetBaseFolder_VST3="{app}"
packageTargetBaseFolder_LV2="{app}"