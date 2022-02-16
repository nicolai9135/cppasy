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
        c_label = 'Rest'
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
print(data_6_td.shape)

combination_list = create_full_combination_list()

data_6_td_rel = np.zeros((1910,5,9,2), dtype=np.float128)
alert_count = 0




portion = np.zeros((1910))
for benchmark in range(data_6_td.shape[1]):
    if benchmark < 5:
        print(benchmark)
        print("total: ",  data_6_td[0, benchmark, 0, 4, 1])
        print("splitting: ", data_6_td[0, benchmark, 4, 4, 1])
        
    total = data_6_td[0, benchmark, 0, 4, 1]
    if total == 0:
        print(benchmark)
    splitting = data_6_td[0, benchmark, 4, 4, 1]
    portion[benchmark] = splitting/total
print("02101 mean: ", np.mean(portion, axis = 0))





for benchmark in range(data_6_td.shape[1]):
    ttime0 = data_6_only[0, benchmark, 0, 0, 1]
    for combination_index in range(len(combination_list)):
        speedup = data_6_only[0, benchmark, 0, combination_index, 1]/ttime0
        divisor = data_6_td[0, benchmark, 0, combination_index, 1]
        if divisor == 0:
            print('ALERT!')
            alert_count += 1
        for measurement in range(data_6_td.shape[2]):
            # copy combination name
            data_6_td_rel[benchmark, measurement, combination_index, 0] = data_6_td[0, benchmark, measurement, combination_index, 0]
            # scale...
            data_6_td_rel[benchmark, measurement, combination_index, 1] = data_6_td[0, benchmark, measurement, combination_index, 1]/divisor
            data_6_td_rel[benchmark, measurement, combination_index, 1] *= speedup 

data_6_td_rel_mean = np.mean(data_6_td_rel, axis=0)
time_dist = np.squeeze(data_6_td_rel_mean)
print(time_dist.shape)
print(alert_count)

        
# time_dist=np.load("time_dist.npy")
# print(time_dist)
# print(time_dist.shape)
# 
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

        c_color, c_label = get_style(timeslice_index)
        plt.bar(range(len(combination_list)), timeslice_size, bottom=offset, color=c_color, edgecolor='white', label=c_label)
        
        if timeslice_index > 0:
            offset += timeslice_size
    plt.xticks(range(len(combination_list)), combination_list)
    plt.legend(loc="lower left")
    plt.show()
        
# plt.bar(r, bars3, bottom=bars, color='#2d7f5e', edgecolor='white', width=barWidth)
 
# Custom X axis
# plt.xlabel("group")
