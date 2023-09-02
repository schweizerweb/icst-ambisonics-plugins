#!/usr/bin/env bash

generate_element() {
    local fullPath=$1
    local elementName=$2
    
    if [[ $elementName == *"."* ]]; then
        local fileDefinition="${fileDefinitionTemplate//__ABSOLUTE_FILEPATH__/$fullPath}"
        echo $fileDefinition
    else
        local fileList=""
        local files=$(find ${fullPath} -depth 1 ! -name ".DS_Store")
        for file in $files; do
            local str="$(generate_element $file "$(basename $file)")"
            fileList+=$str
        done
        
        local folderDefinition=${folderTemplate//__FILESECTION_FOLDERNAME__/$elementName}
        local folderDefinition=${folderDefinition//$fileListMark/$fileList}
        echo $folderDefinition
    fi
}

generate_file_section()
{
    : '
    local packageId=${1}
    local packageVersion=${2}
    local basePath=${3}
    local fileSectionType=${4}
    local fileSection=""

    local packageUuid="$(arrayGet packageUuids $packageId)"
    local packageName="$(arrayGet packageNames $packageId)"
    local packageFolder="$(arrayGet packageTargetFolders $packageId)"
    packageFolder=${packageFolder:=$packageId} # use packageId as default if not defined otherwise 
    
    if [[ "$packageUuid" == "" ]]; then
        >&2 echo "Error: no UUID defined for package $packageId"
        exit
    fi

    case "$fileSectionType" in
     0) fileSection=$fileSectionTemplateTemplates ;;
     1) fileSection=$fileSectionTemplateBinaries ;;
    esac   
														
    result="${mainFileSectionTemplate//__PACKAGE_NAME__/$packageName}"
    result="${result//__PACKAGE_UUID__/$packageUuid}"
    result="${result//__PACKAGE_ID__/$packageId}"
    result="${result//__FILE_SECTION__/$fileSection}"

    local fileList="$(generate_element $basePath $packageFolder)"

    result="${result//$fileListMark/$fileList}"

    if [[ "$fileSectionType" == "0" ]]; then
        newLine='\n'
        generatedScript=$"#!/bin/bash ${newLine}mv /Users/Shared/AmbiPluginsTemplatesTemp/${packageId}/* ~/Library/Application\ Support/REAPER/${packageId}/ ${newLine}rm -r /Users/Shared/AmbiPluginsTemplatesTemp/${packageId}"
        scriptFilename="$(pwd)/post_install_script_${packageId}.sh"
        echo -e $generatedScript > $scriptFilename
        result="${result//__POSTINSTALL_SCRIPT__/$scriptFilename}"
    fi

    echo $result
    '

    echo "file"
}

generate_package_section()
{
    local packageId=${1}
    local desc="$(arrayGet packageNames $packageId)"
    if [[ "$desc" == "" ]]; then
        >&2 echo "Error: No description defined for $packageId"
        exit
    fi

    echo $"Name: \""${packageId}"\"; Description: \""${desc}"\"; Types: custom full \n"
}

write_file()
{
    local content="${1}"
    local filename=${2}
    echo -e $content > $filename
}