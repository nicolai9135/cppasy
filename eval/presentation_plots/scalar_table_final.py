import os
import numpy as np
import re
import math
import sys

rootdir = '/home/nicolai/RWTH/master/master_thesis/evaluation/results_final_leveled_removed_smaller_6'

# create combination list
def create_combination_list(x):
    if x == 0:
        combination_list = ["00000",
                            "00010",
                            "00101",
                            "01000",
                            "01001",
                            "02000",
                            "02101"]
    if x == 1:
        combination_list = ["10000",
                            "12101"]
    return combination_list

def get_dirs():
    dirs = []
    for item in os.listdir(rootdir):
        if os.path.isdir(os.path.join(rootdir, item)):
            dirs.append(item)
    return dirs

def get_result_table(directory, combination_list, current_depth):
    timeout_bool = 0
    too_small_bool = 0

    # prepare resulting table
    result_table = [[0 for x in range(2)] for y in range(len(combination_list))] 
    for i in range(len(combination_list)):
        result_table[i][0]=int(combination_list[i])
    result_table = np.array(result_table)

    # fill absolute result table
    for combination_index in range(len(combination_list)):
        combination = combination_list[combination_index]

        # get max non-timeout-number
        timeout_file = open(rootdir + "/" + directory + "/" + combination + "/" + "timeouts.txt")
        last_line = timeout_file.readlines()[-1]
        max_depth_reached = re.sub("\D", "", last_line)
        max_depth_reached = int(max_depth_reached)
        if "timeout" in last_line:
            max_depth_reached -= 1

        # fill table
        if current_depth > max_depth_reached:
            result_table[combination_index, 1] = -1
        elif (current_depth%2 == 1) and (combination[3] == "1"):
            result_table[combination_index, 1] = -2
        else:
            eval_file = open(rootdir + "/" + directory + "/" + combination + "/" + str(current_depth).zfill(2) + ".txt")
            for current_line in eval_file:
                if "time_total" in current_line:
                    result_table[combination_index, 1] = int(re.sub("\D", "", current_line))

    result_table_relative = result_table.astype(np.float32)

    for combination_index in range(len(combination_list)):
        if (result_table[combination_index, 1] == -1) or (result_table[0, 1] == -1):
            result_table_relative[combination_index, 1] = -1
            timeout_bool = 1
        elif (result_table[combination_index, 1] == -2):
            result_table_relative[combination_index, 1] = -2
        elif (result_table[combination_index, 1] < 5) or (result_table[0, 1] < 5):
            result_table_relative[combination_index, 1] = -3
            too_small_bool = 1
        else:
#            if current_depth > 3:
#                print(result_table[combination_index, 1] , result_table[0, 1])
            result_table_relative[combination_index, 1] = result_table[combination_index, 1] / result_table[0, 1]
#             if current_depth > 3:
                #print(result_table_relative[combination_index, 1])

    return result_table_relative, timeout_bool, too_small_bool

def sort_table(result_table_relative, sort_index, combination_list):
    return np.concatenate(sorted(result_table_relative, key=lambda row: row[sort_index+1])).reshape(int(len(combination_list)), max_depth+1-1)

#####################################################################################################################################

np.set_printoptions(linewidth=np.inf, precision=2, suppress=1, threshold=sys.maxsize)

dirs = get_dirs()

max_depth = 12
# +1 for the 0th element
max_depth += 1


for x in range(2):
    print("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
    print("x = ", x)

    final_table_list = []
    combination_list = create_combination_list(x)
    for current_depth in range(max_depth):
        print("-------------------------------------------------------------------------------")
        print("current_depth = ", current_depth)
        relative_res_list = []
        
        timeout_count = 0
        too_small_count = 0
        # get results
        for directory in dirs:
            result_table_relative, timeout_bool, too_small_bool = get_result_table(directory, combination_list, current_depth)
            if timeout_bool:
                timeout_count += 1
            elif too_small_bool:
                too_small_count += 1
            else:
                relative_res_list.append(result_table_relative)
        print("-> timeouts:  ", timeout_count)
        print("-> too small: ", too_small_count)
        print("-> valid:     ", len(relative_res_list))

        if len(relative_res_list) > 0:
            concatenated = np.array( relative_res_list )
            mean = np.mean(concatenated, axis = 0)
            # print("Mean:")
            # print(mean)
            short = np.delete(mean, [0], 1)
            if current_depth == 1:
                index = np.delete(mean, [1], 1)
                final_table_list.append(index)
            final_table_list.append(short)
            
    print(final_table_list)
    concatenated = np.concatenate( final_table_list, axis=1 )
    print(concatenated)
    print("sorted")
    sorted_con = sort_table(concatenated, 5, combination_list)
    print(sorted_con)
