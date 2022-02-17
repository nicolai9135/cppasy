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

def get_style(timeslice_index):
    if timeslice_index == 0:
        c_color = rwthgray
        c_label = 'Unspecified'
    if timeslice_index == 1:
        c_color = rwthlightgreen
        c_label = 'Solving'
    if timeslice_index == 2:
        c_color = rwthlightblue
        c_label = 'Model Saving'
    if timeslice_index == 3:
        c_color = rwthlightbordeaux
        c_label = 'Sampling'
    if timeslice_index == 4:
        c_color = rwthlightorange
        c_label = 'Splitting'

    return c_color, c_label


np.set_printoptions(linewidth=np.inf, precision=2, suppress=1, threshold=sys.maxsize)

data6 = np.load("data6.npy")

data_6_only = np.delete(data6, [0,1,2,3,4,5], axis=0)

data_6_td = np.delete(data_6_only, [5,6,7,8,9,10,11,12], axis=2)

combination_list = create_full_combination_list()

data_6_td_rel = np.zeros((1910,5,9,2), dtype=np.float128)

for benchmark in range(data_6_td.shape[1]):
    for combination_index in range(len(combination_list)):
        if combination_index < 7:
            ttime0 = data_6_td[0, benchmark, 0, 0, 1]
        else:
            ttime0 = data_6_td[0, benchmark, 0, 7, 1]
        for measurement in range(data_6_td.shape[2]):
            # copy combination name
            data_6_td_rel[benchmark, measurement, combination_index, 0] = data_6_td[0, benchmark, measurement, combination_index, 0]
            # scale...
            data_6_td_rel[benchmark, measurement, combination_index, 1] = data_6_td[0, benchmark, measurement, combination_index, 1]/ttime0

data_6_td_rel_mean = np.mean(data_6_td_rel, axis=0)
time_dist = np.squeeze(data_6_td_rel_mean)

        
# time_dist=np.load("time_dist.npy")
# print(time_dist)
# print(time_dist.shape)
# 
# plot 2 diagrams

spots_0 = [-3,-2,-1,0,1,2,3]
spots_1 = [-0.5,0.5]

for x in [0,1]:
    combination_list = create_combination_list(x)
    offset = np.zeros(len(combination_list))
    # iterate over different timesizes
    for timeslice_index in range(5):
        timeslice_size = np.zeros(len(combination_list))
        for combination_index in range(len(combination_list)):
            combination_index_fix = combination_index
            if x == 1:
                combination_index_fix += len(create_combination_list(0))
            timeslice_size[combination_index] = time_dist[timeslice_index, combination_index_fix, 1]

        c_color, c_label = get_style(timeslice_index)

        if x == 0:
            plt.bar(spots_0, timeslice_size, bottom=offset, color=c_color, edgecolor='white', label=c_label, width=0.8)
        else:
            plt.bar(spots_1, timeslice_size, bottom=offset, color=c_color, edgecolor='white', label=c_label, width=0.8)

        
        if timeslice_index > 0:
            offset += timeslice_size

    if x == 0:
        plt.xticks(spots_0, combination_list)
    else:
        plt.xticks(spots_1, combination_list)

    ax = plt.gca()

    ax.set_ylabel('relative runtime')
    # ax.set_xlabel('time (ms)')

    # set axis limits
    ax.set_xlim([-4,4])
    ax.set_ylim([0, 1.5])

    # remove top and right spines
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

    # add arrow at end of y axis
    ax.plot(0, 1.5, "^k", transform=ax.get_yaxis_transform(), clip_on=False)

    

    handles, labels = plt.gca().get_legend_handles_labels()
    order = [0,4,3,2,1]
    plt.legend([handles[idx] for idx in order], [labels[idx] for idx in order], loc="lower left")

    plt.show()
        
# plt.bar(r, bars3, bottom=bars, color='#2d7f5e', edgecolor='white', width=barWidth)
 
# Custom X axis
# plt.xlabel("group")
