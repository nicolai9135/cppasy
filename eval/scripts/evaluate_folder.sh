#!/bin/bash

max_depth=6

for smt2_file in *.smt2;
do
    # create folder
    echo "$smt2_file"
    folder_name=${smt2_file%.smt2}
    echo creating folder "$folder_name"
    mkdir $folder_name

    # run file for every depth
    for (( depth=0; depth<=$max_depth; depth++ ))
    do
        output_file_name=$(printf "%02d" $depth)
        output_file_name="$folder_name/$output_file_name.txt"
        echo output file name: "$output_file_name"
        ./evaluate_single.sh $smt2_file $depth 0 0 0 0 0 > $output_file_name
    done

done
