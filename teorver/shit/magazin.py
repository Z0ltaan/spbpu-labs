import math

q = 0.005
n = 570

res = 0

for i in range(0,3):
  # res += math.exp(-q) * (q ** i) / math.factorial(i)
  res += math.comb(n, i) * (q ** i) * ((1 - q) ** (n - i))
  print(i)
print(res)

