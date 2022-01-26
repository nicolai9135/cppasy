import os
import numpy as np
import re

rootdir = '/home/nicolai/RWTH/master/master_thesis/evaluation/chosen_results_1'


# create combination list
def create_combination_list():
    combination_list = []
    for sph in range(2):
        for sah in range(3):
            for sm in range (2):
                for e2 in range (2):
                    for ss in range (2):
                        current_combination = ""
                        current_combination += str(sph)
                        current_combination += str(sah)
                        current_combination += str(sm)
                        current_combination += str(e2)
                        current_combination += str(ss)
                        combination_list.append(current_combination)
    return combination_list

def get_dirs():
    dirs = []
    for item in os.listdir(rootdir):
        if os.path.isdir(os.path.join(rootdir, item)):
            dirs.append(item)
    return dirs

def get_files():
    files = []
    for item in os.listdir(rootdir):
        if os.path.isfile(os.path.join(rootdir, item)):
            files.append(item)
    return files

np.set_printoptions(linewidth=np.inf, precision=2, suppress=1)


combination_list = create_combination_list()
dirs = get_dirs()

max_depth = 12
max_depth += 1


for directory in dirs:
    print(directory)

    # prepare resulting table
    result_table = [[0 for x in range(max_depth+1)] for y in range(len(combination_list))] 
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
        for depth in range(max_depth):
            if depth > max_depth_reached:
                result_table[combination_index, depth+1] = -1
            elif (depth%2 == 1) and (combination[3] == "1"):
                result_table[combination_index, depth+1] = -2
            else:
                eval_file = open(rootdir + "/" + directory + "/" + combination + "/" + str(depth).zfill(2) + ".txt")
                for current_line in eval_file:
                    if "time_total" in current_line:
                        result_table[combination_index, depth+1] = int(re.sub("\D", "", current_line))

    
    result_table_relative = result_table.astype(np.float32)
    # fill relative result table
    for combination_index in range(len(combination_list)):
        for depth in range(max_depth):
            if (result_table[combination_index, depth+1] == -1) or (result_table[0, depth+1] == -1):
                result_table_relative[combination_index, depth+1] = -1
            elif (result_table[combination_index, depth+1] == -2):
                result_table_relative[combination_index, depth+1] = -2
            elif (result_table[combination_index, depth+1] < 5) or (result_table[0, depth+1] < 5):
                result_table_relative[combination_index, depth+1] = -3
            else:
                result_table_relative[combination_index, depth+1] = result_table[combination_index, depth+1] / result_table[0, depth+1]

    print(result_table)
    print(result_table_relative)
