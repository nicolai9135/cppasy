#!/bin/bash

max_depth=12
timeout_val=15
file_count=0

directory_recursion () {
    # pwd; echo contains ; ls *.smt2 | wc -l; echo .smt2 files
    # pwd
    pwd

    for f in *; do
        if [ -d "$f" ]; then
            cd $f
            directory_recursion
            cd ..
        fi
    done

    smt2_count=$(ls *.smt2 2>/dev/null | wc -l)

    if [ $smt2_count -ge 1 ]; then 
        for smt2_file in *.smt2;
        do
            ((file_count+=1))
	    echo file number "$file_count"
            benchmark_folder_name=${smt2_file%.smt2}
	    feature_folder_name="$benchmark_folder_name/02000"
	    mkdir $feature_folder_name

	    feature_file_name="$feature_folder_name/timeouts.txt"
	    for (( depth=0; depth<=$max_depth; depth++ ))
	    do
		output_file_name=$(printf "%02d" $depth)
		output_file_name="$feature_folder_name/$output_file_name.txt"
		echo output file name: "$output_file_name"

		timeout $timeout_val /home/nicolai.radke/master_thesis/cppasy-main/eval/scripts/evaluate_single.sh $smt2_file $depth 0 2 0 0 0 > $output_file_name

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
    fi
}

#echo "$1"

if [[ -d "$1" ]]
then
    cd $1
    directory_recursion
else
    echo "$1 is not a valid directory"
fi

echo "evaluated"
echo "$file_count"
echo "benchmarks"
