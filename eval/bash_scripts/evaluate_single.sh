#!/bin/bash

set_splitting_heuristic () {
    if [[ $1 -eq 0 ]]
    then
        splitting_heuristic="bisect_all"
    elif [[ $1 -eq 1 ]]
    then
        splitting_heuristic="bisect_single"
    else
        echo invalid splitting heuristic index
        exit
    fi
}

set_sampling_heuristic () {
    if [[ $1 -eq 0 ]]
    then
        sampling_heuristic="no_sampling"
    elif [[ $1 -eq 1 ]]
    then
        sampling_heuristic="center"
    elif [[ $1 -eq 2 ]]
    then
        sampling_heuristic="clever"
    else
        echo invalid sampling heuristic index
        exit
    fi
}

set_save_model () {
    if [[ $1 -eq 0 ]]
    then
        save_model=" "
    elif [[ $1 -eq 1 ]]
    then
        save_model=" --save-model"
    else
        echo invalid save model index
        exit
    fi
}

set_execute_2in1 () {
    if [[ $1 -eq 0 ]]
    then
        execute_2in1=" "
    elif [[ $1 -eq 1 ]]
    then
        execute_2in1=" --execute-2in1"
    else
        echo invalid execute 2in1 index
        exit
    fi
}

set_split_samples () {
    if [[ $1 -eq 0 ]]
    then
        split_samples=" "
    elif [[ $1 -eq 1 ]]
    then
        split_samples=" --split-samples"
    else
        echo invalid split-sample index
        exit
    fi
}

# $1    benchmark name
# $2    max depth
# $3    splitting_heuristic
# $4    sampling_heuristic
# $5    save_model
# $6    execute_2in1
# $7    split_samples

splitting_heuristic_index=$3
sampling_heuristic_index=$4
save_model_index=$5
execute_2in1_index=$6
split_samples_index=$7

to_eval="/home/nicolai.radke/master_thesis/cppasy-main/eval/scripts/cli"
to_eval="$to_eval $1 --max-depth=$2 --default-boundaries=4"

set_splitting_heuristic $splitting_heuristic_index
to_eval="$to_eval --splitting-heuristic=$splitting_heuristic"

set_sampling_heuristic $sampling_heuristic_index
to_eval="$to_eval --sampling-heuristic=$sampling_heuristic"

set_save_model $save_model_index
to_eval="$to_eval$save_model"

set_execute_2in1 $execute_2in1_index
to_eval="$to_eval$execute_2in1"

set_split_samples $split_samples_index
to_eval="$to_eval$split_samples"

echo "$to_eval"
eval "$to_eval"
