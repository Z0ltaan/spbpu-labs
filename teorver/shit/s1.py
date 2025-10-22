import math

N = 10
p = 0.16
x = 0.78

res = 0
for i in range(0, N - math.ceil(N * x) + 1):
  res += math.comb(N, i) * (p ** i) * ((1 - p) ** (N - i))
  print(i)
print(res)
