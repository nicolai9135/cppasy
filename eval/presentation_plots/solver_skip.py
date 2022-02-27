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

solver_skip = np.load("solver_skip.npy")

print(solver_skip.shape)


combination_list = create_full_combination_list()

res = np.zeros((4, 2*len(combination_list)))

print(res.shape)

for combination_index in range(len(combination_list)):
    combination = combination_list[combination_index]
    res_index = 2*combination_index
    res[0, res_index] = float(combination)
    res[1, res_index] = solver_skip[0,combination_index,1] + solver_skip[3,combination_index,1]
    res[2, res_index] = solver_skip[2,combination_index,1] + solver_skip[3,combination_index,1]
    res[3, res_index] = solver_skip[3,combination_index,1]

for combination_index in range(len(combination_list)):
    combination = combination_list[combination_index]
    res_index_abs = 2*combination_index
    res_index = 2*combination_index+1
    res[0, res_index] = float(combination)
    res[1, res_index] = res[1, res_index_abs]/res[1, res_index_abs]
    res[2, res_index] = res[2, res_index_abs]/res[1, res_index_abs]
    res[3, res_index] = res[3, res_index_abs]/res[1, res_index_abs]

print(res)
