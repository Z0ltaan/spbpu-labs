import math

n1 = 3
n2 = 5
n3 = 2


def comb(k, n):
  return math.factorial(n) / (math.factorial(k) * math.factorial(n - k))

good = comb(2, n1 + n2 + n3) - comb(2, n1) - comb(2, n2)-  comb(2, n3)
print("Good pairs: ",good)

has_gold = n1 * n2 + n1 * n3
print( has_gold / good)
