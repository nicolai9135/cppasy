# libraries
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc

rwthblack='#000000'
rwthlightgreen='#B8D698'
rwthlightblue='#8EBAE5'
rwthblue='#00549F'
rwthlightbordeaux='#CD8B87'
rwthbordeaux='#A11035'
rwthlightpetrol='#7DA4A7'
rwthlightorange='#FDD48F'
rwthgray='#9C9E9F'

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

def get_color(timeslice_index):
    if timeslice_index == 0:
        return rwthgray
    if timeslice_index == 1:
        return rwthlightgreen
    if timeslice_index == 2:
        return rwthlightblue
    if timeslice_index == 3:
        return rwthlightbordeaux
    if timeslice_index == 4:
        return rwthlightorange


np.set_printoptions(linewidth=np.inf, precision=2, suppress=1, threshold=sys.maxsize)

time_dist = np.load("time_dist.npy")

print(time_dist)
print(time_dist.shape)

# plot 2 diagrams
for x in [0,1]:
    combination_list = create_combination_list(x)
    offset = np.zeros(len(combination_list))
    # iterate over different timesizes
    for timeslice_index in range(5):
        print(offset)
        timeslice_size = np.zeros(len(combination_list))
        for combination_index in range(len(combination_list)):
            # print(timeslice_size[combination_index])
            combination_index_fix = combination_index
            if x == 1:
                combination_index_fix += len(create_combination_list(0))
            timeslice_size[combination_index] = time_dist[timeslice_index, combination_index_fix, 1]
        print(timeslice_size)

        plt.bar(range(len(combination_list)), timeslice_size, bottom=offset, color=get_color(timeslice_index), edgecolor='white')
        
        if timeslice_index > 0:
            offset += timeslice_size
    plt.show()
        
# plt.bar(r, bars3, bottom=bars, color='#2d7f5e', edgecolor='white', width=barWidth)
 
# Custom X axis
# plt.xticks(r, names, fontweight='bold')
# plt.xlabel("group")
