import math

n = 26050
p = 0.25
pr = 0.81

def prob(e):
  return 2 * math.sqrt(2/ math.pi) * math.erf(e * math.sqrt(n / (p * (1- p))))

err = 1

while (prob(err) < pr):
  print(err)
  err += 1

print(1.31 / math.sqrt(n * p * (1 - p)))
