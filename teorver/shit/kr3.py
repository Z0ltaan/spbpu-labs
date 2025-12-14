import math

# def m2(x, a):
#   return (math.pi ** 0.5 * a * math.erf(x/(a ** 0.5)) / 4 - a * x / (2 * math.exp(x * x / a)))
a = 168
b = 178
h = 172
s = 8

def rasp(x):
  return 0.5 * (1 + math.erf((x - h)/((2 * s * s) ** 0.5)))


print((1- rasp(b)) / (1 - rasp(a)) * 100)
# print(rasp(b))
# print (a - (b + a) / 2 + (a *a + b * b) * ((b * b + a * b + a * a) / 3 ))
#
# print()
# def p1(x):
#   return 2 * 1.2 * 1.2 ** 2 * x * math.exp(-(1.2 **2 * x ** 2) / (1.2 ** 2))/ (1.2 **2)
# def F(a, t = 2):
#   return (t * a * math.exp(t * a) - math.exp(t * a) + 1) / (a * math.exp(t * a))
# def F(a, t =2):
#   return 1 - math.exp(-a * t)
#
# a = 0.55
# b = 0.45
# c = 0.51
# t = 2
# # print(math.exp(-2.1 * (4.3**0.5 - 0.6**0.5)))
# f1 = F(a) * (1 - F(b)) * (1 - F(c))
# f2 =  F(b) * (1 - F(a)) * (1 - F(c))
# f3 =  F(c) * (1 - F(b)) * (1 - F(a))
# print(f1 + f2 + f3)
