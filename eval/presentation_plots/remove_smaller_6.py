import os
import numpy as np
import re
import math
import sys
import shutil

rootdir = '/home/nicolai/RWTH/master/master_thesis/evaluation/results_final_leveled_removed_smaller_6'

# create combination list
def create_combination_list_0():
    combination_list = ["00000",
                        "00010",
                        "00101",
                        "01000",
                        "01001",
                        "02000",
                        "02101"]
    return combination_list

def create_combination_list_1():
    combination_list = ["10000",
                        "12101"]
    return combination_list

def get_dirs():
    dirs = []
    for item in os.listdir(rootdir):
        if os.path.isdir(os.path.join(rootdir, item)):
            dirs.append(item)
    return dirs

def get_max_common(directory, combination_list):
    # get max depth reached
    global_max = 13
    for combination_index in range(len(combination_list)):
        combination = combination_list[combination_index]

        # get max non-timeout-number
        timeout_file = open(rootdir + "/" + directory + "/" + combination + "/" + "timeouts.txt")
        last_line = timeout_file.readlines()[-1]
        max_depth_reached = re.sub("\D", "", last_line)
        max_depth_reached = int(max_depth_reached)
        if "timeout" in last_line:
            max_depth_reached -= 1
        if max_depth_reached < global_max:
            global_max = max_depth_reached

    return global_max

#####################################################################################################################################

np.set_printoptions(linewidth=np.inf, precision=2, suppress=1, threshold=sys.maxsize)

combination_list_0 = create_combination_list_0()
combination_list_1 = create_combination_list_1()
combined_combination_list = combination_list_0 + combination_list_1

dirs = get_dirs()

max_depth = 12
# +1 for the 0th element
max_depth += 1

total_count = 0
keep_count = 0
remove_count = 0

for directory in dirs:
    dir_full_path = os.path.join(rootdir, directory)
    total_count += 1
    max_common = get_max_common(directory, combined_combination_list)
    if max_common < 6:
        shutil.rmtree(dir_full_path)
        remove_count += 1
        print("removing ", directory, " with max depth max_common", max_common)
    else:
        keep_count += 1
        print("keeping ", directory, " with max depth max_common", max_common)

print("total  ", total_count)
print("keep   ", keep_count)
print("remove ", remove_count)
