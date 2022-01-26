#!/bin/bash

# max depth
max_depth=$1

# timeout
timeout_value=$2

for smt2_file in *.smt2;
do
    /home/nicolai.radke/master_thesis/cppasy-main/eval/scripts/evaluate_all_features.sh $smt2_file $max_depth $timeout_value
done
