#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PLATFORM_FOLDER=${1}
PACKAGE_VERSION=${2}
PACKAGE_NAME=${3}
README_PATH=$SCRIPT_DIR/readme.txt

source $SCRIPT_DIR/$PLATFORM_FOLDER/definitions.sh
source $SCRIPT_DIR/$PLATFORM_FOLDER/file_content_generator.sh

# load template file and prepare output
originalTemplateFile=$SCRIPT_DIR/$PLATFORM_FOLDER/$templateFileName
outputFile="$(pwd)/$outputFileName"
templateFileContent=$(cat ${originalTemplateFile})

###### generate package and files sections
packageSection=""
fileSection=""

# binaries
binaryDir=${SCRIPT_DIR}/../build/bin
binaryFolders=$(find ${binaryDir} -type d -depth 1)
for subDir in $binaryFolders; do
    localPackageSection="$(generate_package_section $(basename $subDir))"
    packageSection+=$localPackageSection

    localFileSection="$(generate_file_section $(basename $subDir) ${PACKAGE_VERSION} $subDir 1)"
    fileSection+=$localFileSection
done

# templates
templateDir=${SCRIPT_DIR}/Templates
templateFolders=$(find ${templateDir} -type d -depth 1)
for subDir in $templateFolders; do
    localPackageSection="$(generate_package_section $(basename $subDir))"
    packageSection+=$localPackageSection

    localFileSection="$(generate_file_section $(basename $subDir) ${PACKAGE_VERSION} $subDir 0)"
    fileSection+=$localFileSection
done

# finally replace template file content and write file
templateFileContent=${templateFileContent//$readmePathMark/$README_PATH}
templateFileContent=${templateFileContent//$packageNameMark/$PACKAGE_NAME}
templateFileContent=${templateFileContent//$installerSectionMark/$packageSection}
templateFileContent=${templateFileContent//$packageSectionMark/$fileSection}

write_file "$templateFileContent" "$outputFile"
echo "----------- build installer done ----------"
exit 0

