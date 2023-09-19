#!/usr/bin/env bash

get_full_path()
{
    local winPath="$(cygpath -m ${1})"
    echo ${winPath//\\/\\\\}
}

generate_file_section()
{
    local packageId=${1}
    local packageVersion=${2}
    local basePath=${3}
    local fileSectionType=${4}
    local fileSection=""

    local packageFolder="$(arrayGet packageTargetFolders $packageId)"
    packageFolder=${packageFolder:=$packageId} # use packageId as default if not defined otherwise 
    
    local packageTargetFolder="$(arrayGet packageTargetBaseFolder $packageId)"
    if [[ "$packageTargetFolder" == "" ]]; then
        >&2 echo "Error: No package target folder defined for $packageId"
        exit
    fi

    local fileList=$(find ${basePath} -mindepth 2 -type f)
    local str=""
    for f in $fileList; do
        local localBasePath="$(dirname $f)"
        local relPath="${packageTargetFolder}/${packageId}${localBasePath#$basePath}"    
        local fp="$(get_full_path $f)"
        str+=$"Source: \"${fp}\"; DestDir: \"${relPath}\"; Flags: ignoreversion; Components: ${packageId}\r\n"
    done

    echo $str
}

generate_package_section()
{
    local packageId=${1}
    local desc="$(arrayGet packageNames $packageId)"
    if [[ "$desc" == "" ]]; then
        >&2 echo "Error: No description defined for $packageId"
        exit
    fi
    
    extraFlags="$(arrayGet packageExtraFlags $packageId)"
    echo $"Name: \""${packageId}"\"; Description: \""${desc}"\"; Types: custom full $extraFlags\r\n"
}

generate_dir_section()
{
    local packageId=${1}
    echo $"Name: \""{userappdata}/REAPER/${packageId}"\"; Components: ${packageId}\r\n"
}

handle_fixed_files()
{
    local content=${1}
    local basePath=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

    local absRedist="\"$(get_full_path "${basePath}/Redist/VC_redist.x64.exe")\""
    echo "${content//$redistMark/$absRedist}"
}

write_file()
{
    local content="${1}"
    local filename=${2}
    echo -e "$content" > $filename
}

build_installer()
{
    iscc ${1}
}
