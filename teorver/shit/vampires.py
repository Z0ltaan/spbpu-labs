#!/bin/python3
import math

p = 0.59

def q(p):
    return 1-p
print(1-(math.factorial(14)/ (math.factorial(7) ** 2)) * p ** 7 * (1-p) ** 7 )
# def bullshit(p):
#     return (math.factorial(n)/ (math.factorial(i) * math.factorial(n - i)))
#
# lsum = 0
# rsum = 0
# for i in range(0, 7):
#     n = 7 + i
#     print(n)
#     rsum += (math.factorial(n)/ (math.factorial(i) * math.factorial(n - i))) * p ** 7 * q(p) ** i
#     lsum += (math.factorial(n)/ (math.factorial(i) * math.factorial(n - i))) * q(p) ** 7 * p ** i
#
# print(lsum + rsum)
