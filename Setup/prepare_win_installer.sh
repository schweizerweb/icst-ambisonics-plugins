#!/usr/bin/env bash

OUTPUT_FILE=$(pwd)/win64_copy.iss
verbose=0

while getopts "h?vf:b:" opt; do
  case "$opt" in
    h|\?)
      show_help
      exit 0
      ;;
    v)  
      verbose=1
      ;;
    f)  
      OUTPUT_FILE=$OPTARG
      ;;
    b)
      BINARY_FOLDER=$OPTARG
      ;;
  esac
done

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
SOURCE_PATH=${SCRIPT_DIR}/win64.iss
if [ $verbose -eq 1 ] 
then
    echo "Copy script:"
    echo "Source path: ${SOURCE_PATH}"
    echo "Destination path: ${OUTPUT_FILE}"
fi

cp $SOURCE_PATH $OUTPUT_FILE

dirs = ""

templateDir=${SCRIPT_DIR}/Templates
find $templateDir -type d -depth 1 -print0 | while IFS= read -d '' subDir
do
    str=""
    
    files=$(find ${subDir} -type f)
    for file in $files; do
        relPath="$(dirname "${file}")"
        relPath=${relPath#"${templateDir}"}
        s="Source: \"${file}\"; DestDir: \"{userappdata}/REAPER${relPath}\"; Flags: ignoreversion"
        str+="${s}\n"
    done

    if [ $verbose -eq 1 ] 
    then
        echo "Handling Directory ${subDir}: "
        echo "-----------------------"
        echo $str
        echo "-----------------------"
    fi


done 

