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

data_6_tt = np.delete(data_6_only, [1,2,3,4,5,6,7,8,9,10,11,12], axis=2)

combination_list = create_full_combination_list()

speedup = np.zeros((1910, 9, 2))

for benchmark in range(data_6_tt.shape[1]):
    for combination_index in range(len(combination_list)):
        # set compare-time
        if combination_index < 7:
            tt0 = data_6_tt[0, benchmark, 0, 0, 1]
        else:
            tt0 = data_6_tt[0, benchmark, 0, 7, 1]
        # copy combination name
        speedup[benchmark, combination_index, 0] = data_6_tt[0, benchmark, 0 , combination_index, 0]
        speedup[benchmark, combination_index, 1] = data_6_tt[0, benchmark, 0 , combination_index, 1]/tt0

# plot 2 diagrams
spots_0 = [-3,-2,-1,0,1,2,3]
# spots_0 = [-2,-1,0,1,2,3]
spots_1 = [-0.5,0.5]
# spots_1 = [0.5]

for x in [0,1]:
    combination_list = create_combination_list(x)

    for combination_index in range(len(combination_list)):
        if combination_index != 0 and combination_index != 7:
            if x == 1:
                combination_index += len(create_combination_list(0))

            combination_data = np.array(speedup[:,combination_index,1])

            medianprops = dict(linewidth=2.5, color=rwthgray)

            if x == 0:
                plt.boxplot(combination_data, positions = [spots_0[combination_index]], showfliers=False, whis=[5,95], widths=[0.5], medianprops = medianprops)
                # bp = plt.boxplot(combination_data, positions = [spots_0[combination_index]], showfliers=False, whis=[5,95], widths=[0.5], medianprops = medianprops, patch_artist=True)
                # for box in bp['boxes']:
                #     box.set(color='white', linewidth=0)
                #     box.set(facecolor=rwthgray)
            else:
                plt.boxplot(combination_data, positions = [spots_1[combination_index-len(create_combination_list(0))]], showfliers=False, whis=[5,95], widths=[0.5], medianprops = medianprops)
                # plt.boxplot(combination_data, positions = [spots_1[combination_index-len(create_combination_list(0))]], showfliers=False, whis=[5,95], widths=[0.5])

    
    if x == 0:
        plt.xticks(spots_0[1:], combination_list[1:])
    else:
        plt.xticks(spots_1[1:], combination_list[1:])

    ax = plt.gca()

    ax.set_ylabel('relative runtime')
    y_labels = [0.0,0.5,1.0,1.5,2.0]
    y_ticks = y_labels
    plt.yticks(y_ticks, y_labels)

    # set axis limits
    ax.set_xlim([-3,4])
    ax.set_ylim([0, 2.25])

    # remove top and right spines
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

    plt.axhline(y=1, color=rwthlightblue, linestyle='-', linewidth=2.5 )

    # add arrow at end of y axis
    ax.plot(0, 2.25, "^k", transform=ax.get_yaxis_transform(), clip_on=False)

    plt.show()
