#!/bin/bash

max_depth=2
timeout_val=1

for smt2_file in *.smt2;
do
    # timeout $timeout_val /home/nicolai/RWTH/master/master_thesis/cppasy/eval/scripts/evaluate_single.sh $smt2_file $max_depth 0 0 0 0 0
    # timeout $timeout_val is_interesting=$(/home/nicolai/RWTH/master/master_thesis/cppasy/eval/scripts/cli $smt2_file --splits-needed)
    is_interesting=$(timeout $timeout_val /home/nicolai/RWTH/master/master_thesis/cppasy/eval/scripts/cli $smt2_file --splits-needed --default-boundaries=10000)


    # remove file if timed out
    exit_status=$?
    if [[ $exit_status -eq 124 ]]; then
        rm $smt2_file
        echo - timeout: removing "$smt2_file"
    elif [[ $is_interesting -eq 0 ]]; then
        echo - uninteresting: removing "$smt2_file"
    else
        echo - keeping "$smt2_file"
    fi
done
