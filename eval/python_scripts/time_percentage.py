import numpy as np
import matplotlib.pyplot as plt
import sys
# from mpl_toolkits.axisartist.axislines import SubplotZero

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

rwthblack='#000000'
rwthlightgreen='#B8D698'
rwthlightblue='#8EBAE5'
rwthblue='#00549F'
rwthlightbordeaux='#CD8B87'
rwthbordeaux='#A11035'
rwthlightpetrol='#7DA4A7'
rwthlightorange='#FDD48F'
rwthgray='#9C9E9F'

def get_style(c):
    marker = '.'
    color = 'r'
    label = None
    if c == 0:
        marker = 'o'
        color = rwthlightblue
        label = '00000'
    if c == 1:
        marker = 'o'
        color = rwthgray
    if c == 2:
        marker = 'o'
        color = rwthgray
    if c == 3:
        marker = 'o'
        color = rwthgray
    if c == 4:
        marker = 'o'
        color = rwthgray
    if c == 5:
        marker = 'o'
        color = rwthgray
    if c == 6:
        marker = 'o'
        color = rwthlightgreen
        label = '02101'
    if c == 7:
        marker = '^'
        color = rwthlightblue
        label = '10000'
    if c == 8:
        marker = '^'
        color = rwthlightgreen
        label = '12101'
    
    return marker, color, label

np.set_printoptions(linewidth=np.inf, precision=2, suppress=1, threshold=sys.maxsize)

combination_list = create_full_combination_list()

tp_data = np.load("time_percentage.npy")

print(np.shape(tp_data))
print(tp_data)



for combination_index in [1,2,3,4,5,0,6,7,8,-1]:
    if combination_index == -1:
        plt.scatter([],[], marker='o', c=rwthgray, label='other')
    else:
        combination = combination_list[combination_index]
        
        mean_ts = []
        mean_ps = []

        for depth in range(0,7):
            mean_ts.append(tp_data[depth, 0, combination_index, 1])
            mean_ps.append(1-tp_data[depth, 1, combination_index, 1])

        print(combination)
        print("time ", mean_ts)
        print("perc ", mean_ps)
        c_marker, c_color, c_label = get_style(combination_index)
        # plt.scatter(np.array(mean_ts), np.array(mean_ps), marker=c_marker, c=c_color)
        # plt.show

        print("ts ", np.array(mean_ts))
        print("ps ", np.array(mean_ps))

        x=np.array(mean_ts)
        y=np.array(mean_ps)
        y=y*100
        
        plt.scatter(x,y, marker=c_marker, c=c_color, label=c_label)

ax = plt.gca()

ax.set_ylabel('classified area in %')
ax.set_xlabel('time (ms)')

# set axis limits
ax.set_xlim([0, 1250])
ax.set_ylim([50, 100])

# remove top and right spines
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)

# add arrow at end of y axis
ax.plot(1, 50, ">k", transform=ax.get_yaxis_transform(), clip_on=False)

plt.legend(loc="lower right")

# plot diagram
plt.show()

