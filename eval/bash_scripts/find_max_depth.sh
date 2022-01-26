#!/bin/bash

if [[ -d "$1" ]]
then
    cd $1
    # loop through all selected benchmark folders
    for f in *; do
	# check whether it actually is a folder
        if [ -d "$f" ]; then
            cd $f
                pwd
                max_depth=0
                max_depth_combination=0
		# loop through all combination folders
                for f in *; do
                    cd $f
                    current_depth=$(ls *.txt | wc -l)
		    cd ..
		    if [ $current_depth -ge $max_depth ]; then
                        max_depth=$current_depth
			max_depth_combination=$f
                    fi 
                done
		echo "$max_depth"
		echo "$max_depth_combination"
            cd ..
        fi
    done
else
    echo "$1 is not a valid directory"
fi
