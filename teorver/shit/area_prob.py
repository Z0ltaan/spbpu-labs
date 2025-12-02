x1 = 0
x2 = 2

y1 = 2
y2 = 5

a = 3


def distr(x, y):
  base = a
  return 1 - base **(-x) - base ** (-y) + base ** (- (x + y))

print("Answer: ", distr(x2, y2) - distr(x2, y1) - distr(x1, y2) + distr(x1, y1))
