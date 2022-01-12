#!/bin/bash

max_depth=5
timeout_val=15

for smt2_file in *.smt2;
do
    echo "$smt2_file"
    # run file for every depth
    for (( depth=0; depth<=$max_depth; depth++ ))
    do
        timeout $timeout ./evaluate_single.sh $smt2_file $depth 0 0 0 0 0 >/dev/null

        # remove file if timed out
        exit_status=$?
        if [[ $exit_status -eq 124 ]]; then
            rm $smt2_file
            echo - removing "$smt2_file"
            break 2
        fi
    done
    echo - keeping "$smt2_file"
done
