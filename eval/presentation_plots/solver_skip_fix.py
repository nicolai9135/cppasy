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

np.set_printoptions(linewidth=np.inf, precision=2, suppress=1, threshold=sys.maxsize)

data6 = np.load("data6.npy")
data_6_only = np.delete(data6, [0,1,2,3,4,5], axis=0)

data_6_ss = np.delete(data_6_only, [0,1,2,3,4,5,10,11,12], axis=2)
print(data_6_ss.shape)

combination_list = create_full_combination_list()

data_6_ss_rel = np.zeros((1910,4,9,2), dtype=np.float128)
alert_count = 0

for benchmark in range(data_6_ss.shape[1]):
    for combination_index in range(len(combination_list)):
        divisor = data_6_ss[0, benchmark, 0, combination_index, 1]+data_6_ss[0, benchmark, 3, combination_index, 1]
        if divisor == 0:
            print('ALERT!')
        for measurement in range(data_6_ss.shape[2]):
            # copy combination name
            data_6_ss_rel[benchmark, measurement, combination_index, 0] = data_6_ss[0, benchmark, measurement, combination_index, 0]
            # scale...
            data_6_ss_rel[benchmark, measurement, combination_index, 1] = data_6_ss[0, benchmark, measurement, combination_index, 1]/divisor

data_6_ss_rel_mean = np.mean(data_6_ss_rel, axis=0)
solver_skip = np.squeeze(data_6_ss_rel_mean)
print(solver_skip.shape)
print(alert_count)

res = np.zeros((4, len(combination_list)))

print(res.shape)

for combination_index in range(len(combination_list)):
    combination = combination_list[combination_index]
    res_index = combination_index
    res[0, res_index] = float(combination)
    res[1, res_index] = solver_skip[0,combination_index,1] + solver_skip[3,combination_index,1]
    res[2, res_index] = solver_skip[2,combination_index,1] + solver_skip[3,combination_index,1]
    res[3, res_index] = solver_skip[3,combination_index,1]

print(res)

