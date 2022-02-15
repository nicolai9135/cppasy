import os
import numpy as np
import re
import math
import sys

rootdir = '/home/nicolai/RWTH/master/master_thesis/evaluation/results_final_leveled_removed_smaller_6'

def create_full_combination_list():
    combination_list = ["00000",
                        "00010",
                        "00101",
                        "01000",
                        "01001",
                        "02000",
                        "02101",
                        "10000",
                        "12101"]
    return combination_list

def get_dirs():
    dirs = []
    for item in os.listdir(rootdir):
        if os.path.isdir(os.path.join(rootdir, item)):
            dirs.append(item)
    return dirs

def timed_out(directory, combination_list, current_depth):
    global_max_depth_reached = 12
    for combination_index in range(len(combination_list)):
        combination = combination_list[combination_index]

        # get max non-timeout-number
        timeout_file = open(rootdir + "/" + directory + "/" + combination + "/" + "timeouts.txt")
        last_line = timeout_file.readlines()[-1]
        max_depth_reached = re.sub("\D", "", last_line)
        max_depth_reached = int(max_depth_reached)
        if "timeout" in last_line:
            max_depth_reached -= 1
        if max_depth_reached < global_max_depth_reached:
            global_max_depth_reached = max_depth_reached

    return global_max_depth_reached


def get_partial_table(directory, combination_list, current_depth, feature_string):

    # prepare resulting table
    partial_table = np.empty((len(combination_list), 2), dtype=np.float32)

    # fill 1st col with combinations
    for i in range(len(combination_list)):
        partial_table[i][0]=int(combination_list[i])


    # fill table
    for combination_index in range(len(combination_list)):
        combination = combination_list[combination_index]

        # in case incremental was chosen:
        if (current_depth%2 == 1) and (combination[3] == "1"):
            partial_table[combination_index, 1] = -2
        else:
            eval_file = open(rootdir + "/" + directory + "/" + combination + "/" + str(current_depth).zfill(2) + ".txt")
            for current_line in eval_file:
                if feature_string in current_line:
                    partial_table[combination_index, 1] = float(re.findall(r"[-+]?(?:\d*\.\d+|\d+)", current_line)[0])

    return partial_table

def get_full_table(directory, combination_list, current_depth):
    timeout_bool = 0

    full_table_list = []

    # return if timeout appeared in one of the combinations
    max_depth_reached = timed_out(directory, combination_list, current_depth)
    if current_depth > max_depth_reached:
        timeout_bool = 1
        return np.array(full_table_list, dtype=np.float32), timeout_bool

    # 00
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "time_total"))
    # 01
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "time_solving"))
    # 02
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "time_model_saving"))
    # 03
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "time_sampling_total"))
    # 04
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "time_splitting_total"))
    # 05
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "time_splitting_gen_sc_total"))

    # 06
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "solver_activation_count"))
    # 07
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "solver_necessary_count"))
    # 08
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "solver_preventable_count"))
    # 09
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "solver_skip_count"))

    # 10
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "areas_unknown"))
    # 11
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "areas_safe"))
    # 12
    full_table_list.append(get_partial_table(directory, combination_list, current_depth, "areas_unsafe"))

    full_table = np.array(full_table_list)
    return full_table, timeout_bool

def sort_table(result_table_relative, sort_index, combination_list):
    return np.concatenate(sorted(result_table_relative, key=lambda row: row[sort_index+1])).reshape(int(len(combination_list)), max_depth+1-1)

# def plot_tp(combination_list, time_percentage_data):
    

#####################################################################################################################################

np.set_printoptions(linewidth=np.inf, precision=2, suppress=1, threshold=sys.maxsize)

dirs = get_dirs()

max_depth = 12
# +1 for the 0th element
max_depth += 1

# for x in range(2):
#     print("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
#     print("x = ", x)

combination_list = create_full_combination_list()

all_depth_tp_list = []

for current_depth in range(max_depth):
    print("-------------------------------------------------------------------------------")
    print("current_depth = ", current_depth)

    single_depth_list = []
    timeout_count = 0
    too_small_count = 0

    # get results
    for directory in dirs:
        full_table, timeout_bool = get_full_table(directory, combination_list, current_depth)
        if timeout_bool:
            timeout_count += 1
        else:
            single_depth_list.append(full_table)

    single_depth_data = np.array(single_depth_list)
    print("Shape: ", np.shape(single_depth_data))
    print("-> timeouts:  ", timeout_count)
    print("-> valid:     ", len(single_depth_list))
    
    mean = np.mean(single_depth_data, axis=0)
    print("Mean Shape: ", np.shape(mean))
    
    # get only time (0) , and percentage (10)
    time_percentage = np.delete(mean, [1,2,3,4,5,6,7,8,9,11,12], axis=0)
    print("Time Percentage Shape: ", np.shape(time_percentage))
    if current_depth < 7:
        all_depth_tp_list.append(time_percentage)

all_depth_tp = np.array(all_depth_tp_list)
print(np.shape(all_depth_tp))
np.save("time_percentage.npy", all_depth_tp)
