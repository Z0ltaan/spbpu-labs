import math

p1 = 0.055
p2 = 0.85

print (f"p1: {p1 - 0.5}  p2: {p2 - 0.5}")

x1 = -1.59819
x2 = 1.03643

xp1 = -280.5
xp2 = -252.5

xx = x1 / x2

m = (xp1 - xx * xp2) / (1 - xx)
sig = (xp2 - m) / x2
print("m: ", m, "sig: ", sig)


