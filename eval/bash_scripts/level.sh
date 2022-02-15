#!/bin/bash

folder_count=0

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
            benchmark_folder_name=${smt2_file%.smt2}
            echo copying folder "$benchmark_folder_name"
	    ((folder_count+=1))
            cp -r $benchmark_folder_name /home/nicolai/RWTH/master/master_thesis/evaluation/results_final_leveled
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

echo "copied"
echo "$folder_count"
echo "folders"
