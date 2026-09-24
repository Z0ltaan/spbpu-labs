import numpy as np
from scipy import special

def q_function_erf(x):
    return 0.5 - 0.5 * special.erf(x / np.sqrt(2))

def print_res(g_x, deviation, means):
  list = []
  for mean in means:
    list.append(q_function_erf((g_x - mean) / deviation))

  print(*list)

deviation_2g = 6
deviation_4g = 4
deviation_wifi = 3

mean2g = [-57.8000,-60.2000,-52.8000 ]
mean4g = [-71.0000,-68.8000,-69.0000 ]
mean_wifi = [-51,-49.2,-49.2]
g_x = -100
wifi_x = -90

print_res(g_x=g_x, deviation=deviation_2g, means=mean2g)
print_res(g_x=g_x, deviation=deviation_4g, means=mean4g)
print_res(g_x=wifi_x, deviation=deviation_2g, means=mean4g)
