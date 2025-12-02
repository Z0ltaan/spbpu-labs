a = -0.5
b = 1.5 
c = 8

def distr1(x):
  return 1/(2*c**2)*(x + c)**2

def distr2(x):
  return 1 - 1 / (2 * c **2) * (c - x)**2


print((1 - distr1(b) ) / ( 1 - distr2(a)))
