import math

m = 2
n = 8
p = 0.16

res = 0
res += 1 - (1-p) ** n - n * p * (1- p) ** (n-1)
# for i in range(0,3):
# res += math.exp(-n*p) * ((n*p) ** m) / math.factorial(m)
# res += math.comb(n, m) * (p ** m) * ((1 - p) ** (n - m))
  # print(i)
print(res)
