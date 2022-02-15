#!/bin/bash

# $1: benchmark name
smt2_file=$1

# $2: max depth
max_depth=$2

# $3: timeout value
timeout_val=$3

# create benchmark folder
echo "$smt2_file"
benchmark_folder_name=${smt2_file%.smt2}
echo creating folder "$benchmark_folder_name"
mkdir $benchmark_folder_name


# iterate through all combinations
# splitting heuristic
for(( sph=0; sph<2; sph++ ))
do
    # sampling_heuristic
    for(( sah=0; sah<3; sah++ ))
    do
	# save model
        for(( sm=0; sm<2; sm++ ))
        do
            # execute 2 in 1
            for(( e2=0; e2<2; e2++ ))
            do
                # save sample
            	for(( ss=0; ss<2; ss++ ))
            	do
                    # create feature folder
                    feature_folder_name="$benchmark_folder_name/$sph$sah$sm$e2$ss"
                    feature_file_name="$feature_folder_name/timeouts.txt"
	            echo creating folder "$feature_folder_name"
	            mkdir $feature_folder_name

		    # depth
                    for (( depth=0; depth<=$max_depth; depth++ ))
                    do
                        output_file_name=$(printf "%02d" $depth)
                        output_file_name="$feature_folder_name/$output_file_name.txt"
                        echo output file name: "$output_file_name"

                        timeout $timeout_val /home/nicolai.radke/master_thesis/cppasy-main/eval/scripts/evaluate_single.sh $smt2_file $depth $sph $sah $sm $e2 $ss > $output_file_name
                    
                        # remove file if timed out
                        exit_status=$?
                        if [[ $exit_status -eq 124 ]]; then
                            echo depth "$depth": timeout >> $feature_file_name
			    break
                        else
                            echo depth "$depth": finished >> $feature_file_name
                        fi

                    done
            	done
            done
        done
    done
done








