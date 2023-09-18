#!/bin/bash
verbose=0
usage() {                                 # Function: Print a help message.
  echo "Usage: $0 [ -f FOLDER ] [ -l FILE_CONTAINING_LICENSE_INFO (optional) ]" 1>&2 
}
exit_with_error() {
    usage
    exit 1
}

# set default header file
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
HEADER_FILE="${SCRIPT_DIR}/CommonFileHeader.txt"

while getopts "h?f:l:v" opt; do
  case "$opt" in
    h|\?)
      usage
      exit 0
      ;;
    f)
      FOLDER=$OPTARG
      ;;
    l)
      HEADER_FILE=$OPTARG
      ;;
    v)
      verbose=1
      ;;
  esac
done


: ${FOLDER:?"Missing option -f (folder) $(exit_with_error)"}

commonHeader=$(<$HEADER_FILE)
knownCodeStarts=("#pragma" "#include" "//[Headers]")

if [[ verbose -eq 1 ]]; then
    echo "Replacing all headers by:"
    echo "$commonHeader"
fi 

files=$(find $FOLDER -type f \( -name "*.h" -o -name "*.cpp" \))
for file in $files; do 
    if [[ verbose -eq 1 ]]; then 
        echo "file: ${file}"
    fi

    minIndex=0
    found=0
    content=$(<$file)
    for codeStart in ${knownCodeStarts[@]}; do
        prefix=${content%%$codeStart*}
        index=${#prefix}
        if [[ index -eq ${#content} ]]; then
            if [[ verbose -eq 1 ]]; then 
                echo "$codeStart not found."
            fi
        else
            if [[ verbose -eq 1 ]]; then 
                echo "Index of substring in string : $index"
            fi
            if [[ minIndex -eq 0 || minIndex -gt index ]]; then
                minIndex=$index
                found=1
            fi
        fi
    done

    if [[ found -eq 0 ]]; then
        >&2 echo "No code start found for file ${file}"
    else
        if [[ verbose -eq 1 ]]; then 
            echo "$commonHeader"
            echo -e "\n\n"
            echo "${content:minIndex}"
        fi

        echo "$commonHeader" > $file
        echo -e "\n\n" >> $file
        echo "${content:minIndex}" >> $file
    fi
done