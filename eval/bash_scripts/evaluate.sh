#!/bin/bash

# max depth
max_depth=$2

# timeout
timeout_value=$3

directory_recursion () {
    # pwd; echo contains ; ls *.smt2 | wc -l; echo .smt2 files
    # pwd
    pwd
    ls *.smt2 2>/dev/null | wc -l

    
    for f in *; do
        if [ -d "$f" ]; then
            cd $f
            directory_recursion
            cd ..
        fi
    done

    /home/nicolai.radke/master_thesis/cppasy-main/eval/scripts/evaluate_folder.sh $max_depth $timeout_value
}

#echo "$1"

if [[ -d "$1" ]]
then
    cd $1
    directory_recursion
else
    echo "$1 is not a valid directory"
fi
