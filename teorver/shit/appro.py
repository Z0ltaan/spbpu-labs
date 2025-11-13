import math

p = 0.62
z = 0.75
m = 360
n = m

def q(p):
  return 1 - p

def x1(n):
  return (m - n * p) / math.sqrt(n * p * q(p))

def x2(n):
  return math.sqrt(n) * p ** (-0.5)

def pn(n):
  return 0.5 * (math.erf(x2(n) / math.sqrt(2)) - math.erf(x1(n) / math.sqrt(2)))

prob = pn(n)
while ( prob < z):
  print(n)
  n += 1
  prob = pn(n)

print("prob: ", prob)
