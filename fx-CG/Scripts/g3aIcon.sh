#!/bin/bash
if [ $1 ]; then
    input_file=$1
else
    path=$0
    file=${path##*/}
    base=${file%.sh}

    echo usage: "${base}" input_file
    exit
fi

function extract_icon {
    icon_file=$1
    echo -n -e "\x42\x4D\x48\x2E\x00\x00\x00\x00\x00\x00\x46\x00\x00\x00\x38\x00\x00\x00\x5C\x00" > "${icon_file}.bmp"
    echo -n -e "\x00\x00\xC0\xFF\xFF\xFF\x01\x00\x10\x00\x03\x00\x00\x00\x02\x2E\x00\x00\x12\x0B" >> "${icon_file}.bmp"
    echo -n -e "\x00\x00\x12\x0B\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xF8\x00\x00\xE0\x07" >> "${icon_file}.bmp"
    echo -n -e "\x00\x00\x1F\x00\x00\x00\x00\x00\x00\x00" >> "${icon_file}.bmp"
    
    dd skip=$2 count=11776 if="${input_file}" bs=1 2>/dev/null of="${icon_file}.raw"
    
    # Convert endianess
    xxd -p -c 2 "${icon_file}.raw" | sed 's/\(..\)\(..\)/\2\1/g' | xxd -r -p >> "${icon_file}.bmp"
    rm "${icon_file}.raw"
}

extract_icon "icon~uns" 4096
extract_icon "icon~sel" $((4096 + 12288))
