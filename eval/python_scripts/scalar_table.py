import os
import numpy as np
import re
import math
import sys

rootdir = '/home/nicolai/RWTH/master/master_thesis/evaluation/chosen_run2'

# create combination list
def create_combination_list_old():
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

def create_combination_list():
    combination_list = []
    for sph in range(2):
        for sah in range(3):
            for sm in range (2):
                for e2 in range (2):
                    current_combination = ""
                    current_combination += str(sph)
                    current_combination += str(sah)
                    current_combination += str(sm)
                    current_combination += str(e2)
                    if sah == 1 or sm == 1:
                        current_combination += "1"
                    else:
                        current_combination += "0"
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

def get_result_tables(directory, combination_list):
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

    # split result_table
    result_tables_split = np.vsplit(result_table, 2)
    result_tables_relative = []

    for result_table in result_tables_split:
        result_table_relative = result_table.astype(np.float32)
        # fill relative result table
        for combination_index in range(int(len(combination_list)/2)):
            for depth in range(max_depth):
                if (result_table[combination_index, depth+1] == -1) or (result_table[0, depth+1] == -1):
                    result_table_relative[combination_index, depth+1] = -1
                elif (result_table[combination_index, depth+1] == -2):
                    result_table_relative[combination_index, depth+1] = -2
                elif (result_table[combination_index, depth+1] < 5) or (result_table[0, depth+1] < 5):
                    result_table_relative[combination_index, depth+1] = -3
                else:
                    result_table_relative[combination_index, depth+1] = result_table[combination_index, depth+1] / result_table[0, depth+1]
        result_tables_relative.append(result_table_relative)

    return result_tables_split, result_tables_relative

def get_max_common_even_index(result_table_relative, max_depth, combination_list):
    # get lowest common -1 for each table
    first_timeout_col = -1
    for depth in range(max_depth):
        for combination_index in range(int(len(combination_list)/2)):
            if (result_table_relative[combination_index, depth+1] == -1):
                first_timeout_col = depth
                break
        else:
            continue
        break

    max_common_index = -1
    # decrement to get last coloumn != -1
    if first_timeout_col == -1:
        max_common_index = 12
    else:
        max_common_index = first_timeout_col - 1

    # make even:
    max_common_even_index = math.floor(max_common_index/2)*2
    return max_common_even_index

def sort_table(result_table_relative, sort_index, combination_list):
    return np.concatenate(sorted(result_table_relative, key=lambda row: row[sort_index+1])).reshape(int(len(combination_list)/2), max_depth+1)

#####################################################################################################################################

np.set_printoptions(linewidth=np.inf, precision=2, suppress=1, threshold=sys.maxsize)
combination_list = create_combination_list()
dirs = get_dirs()

max_depth = 12
# +1 for the 0th element
max_depth += 1

res_sph_0 = []
res_sph_1 = []

for directory in dirs:
    print("===================================================================================================================")
    print("===================================================================================================================")
    print("Benchmark:")
    print(directory)
    print("Results:")
    result_tables, result_tables_relative = get_result_tables(directory, combination_list)
    for result_table in result_tables:
        print(result_table)

    print("-------------------------------------------------------------------------------------------------------------------")
    print("-------------------------------------------------------------------------------------------------------------------")
    
    sph_index = 0
    for result_table_relative in result_tables_relative:
        print("Relative Results:")
        print(result_table_relative)
    
        sort_index = get_max_common_even_index(result_table_relative, max_depth, combination_list)
        if sort_index < 6:
            print("Problem: sort-index < 6: too less data to make statements")
            break

        if sph_index == 0:
            res_sph_0.append(result_table_relative)
        else:
            res_sph_1.append(result_table_relative)
        sph_index += 1

        # result_tables_relative_list[sph_index].append(result_table_relative)
        
        result_table_relative_sorted = sort_table(result_table_relative, sort_index, combination_list)

        print("Relative Results Sorted by index ", sort_index)
        print(result_table_relative_sorted)
        print("-------------------------------------------------------------------------------------------------------------------")
        
print("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
print("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
print("Average:")

result_tables_relative_list = [res_sph_0, res_sph_1]
for result_table_relative_list in result_tables_relative_list:
    # concatenated = np.concatenate( result_table_relative_list, axis=0 )
    concatenated = np.array( result_table_relative_list )
    mean = np.mean(concatenated, axis = 0)
    print("Mean:")
    print(mean)
    sorted_mean = sort_table(mean, 6, combination_list)
    print("Sorted Mean:")
    print(sorted_mean)
