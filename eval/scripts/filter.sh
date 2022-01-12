#!/bin/bash

directory_recursion () {
    # pwd; echo contains ; ls *.smt2 | wc -l; echo .smt2 files
    # pwd
    ls *.smt2 2>/dev/null | wc -l
    for f in *; do
        if [ -d "$f" ]; then
            cd $f
            directory_recursion
            cd ..
        fi
    done
}

#echo "$1"

if [[ -d "$1" ]]
then
    cd $1
    directory_recursion
else
    echo "$1 is not a valid directory"
fi
