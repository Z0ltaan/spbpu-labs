#!/usr/bin/env python3

from scipy.optimize import line_search
import numpy as np
import time
from algo import hssh1_algo
import matplotlib.pyplot as plt


def cosine_func(x):
  return np.sum(np.cos(x))


def cosine_grad(x):
  return -np.sin(x)


def dixmaana(x):
  n = len(x)
  alpha = 1.0
  gamma = 0.125
  m = 5

  f = 1.0 + alpha * np.sum(x**2)

  idx = np.arange(n - m)
  f += gamma * np.sum(x[idx] ** 2 * x[idx + m] ** 4)
  return f


def dixmaana_grad(x):
  n = len(x)
  alpha = 1.0
  gamma = 0.125
  m = 5
  grad = 2.0 * alpha * x

  for i in range(n - m):
    grad[i] += 2.0 * gamma * x[i] * x[i + m] ** 4
    grad[i + m] += 4.0 * gamma * x[i] ** 2 * x[i + m] ** 3
  return grad


def raydan2(x):
  return np.sum(np.exp(x) - x)


def raydan2_grad(x):
  return np.exp(x) - 1.0


def genquartic(x):
  return np.sum(x**4 + x**2)


def genquartic_grad(x):
  return 4.0 * x**3 + 2.0 * x


def liarwhd(x):
  return np.sum(4.0 * (x**2 - x) ** 2 + (x - 1.0) ** 2)


def liarwhd_grad(x):
  return 8.0 * (x**2 - x) * (2.0 * x - 1.0) + 2.0 * (x - 1.0)


def himmelbg(x):
  return np.sum((x**2 - 1.0) ** 2)


def himmelbg_grad(x):
  return 4.0 * x * (x**2 - 1.0)


def exdenschnf(x):
  f = 0.0
  for i in range(0, len(x) - 1, 2):
    x1, x2 = x[i], x[i + 1]
    term1 = (x1 - 2) ** 4
    term2 = (x1 - 2) ** 2 * (x2 - 1) ** 2
    term3 = (x2 - 1) ** 4
    f += term1 + term2 + term3
  return f


def exdenschnf_grad(x):
  grad = np.zeros_like(x)
  for i in range(0, len(x) - 1, 2):
    x1, x2 = x[i], x[i + 1]
    grad[i] = 4 * (x1 - 2) ** 3 + 2 * (x1 - 2) * (x2 - 1) ** 2
    grad[i + 1] = 2 * (x1 - 2) ** 2 * (x2 - 1) + 4 * (x2 - 1) ** 3
  return grad


def powell(x):
  f = 0.0
  for i in range(0, len(x) // 4):
    f += (
      (x[4 * i] + 10 * x[4 * i + 1]) ** 2
      + 5 * (x[4 * i + 2] - x[4 * i + 3]) ** 2
      + (x[4 * i + 1] - 2 * x[4 * i + 2]) ** 4
      + 10 * (x[4 * i] - x[4 * i + 3]) ** 4
    )
  return f


def powell_grad(x):
  grad = np.zeros_like(x)
  for i in range(0, len(x) // 4):
    grad[4 * i] = (
      2 * (x[4 * i] + 10 * x[4 * i + 1]) + 40 * (x[4 * i] - x[4 * i + 3]) ** 3
    )
    grad[4 * i + 1] = (
      20 * (x[4 * i] + 10 * x[4 * i + 1])
      + 4 * (x[4 * i + 1] - 2 * x[4 * i + 2]) ** 3
    )
    grad[4 * i + 2] = (
      10 * (x[4 * i + 2] - x[4 * i + 3])
      - 8 * (x[4 * i + 1] - 2 * x[4 * i + 2]) ** 3
    )
    grad[4 * i + 3] = (
      -10 * (x[4 * i + 2] - x[4 * i + 3]) - 40 * (x[4 * i] - x[4 * i + 3]) ** 3
    )
  return grad


def run_cg_comparison(f, grad_f, x0, method='DY', tol=1e-6, max_iter=2000):
  start_time = time.process_time()
  x_k = x0.astype(np.float64)
  g_k = grad_f(x_k)
  d_k = -g_k

  g_norm = np.linalg.norm(g_k, ord=np.inf)

  for k in range(max_iter):
    g_norm = np.linalg.norm(g_k, ord=np.inf)

    if g_norm < tol:
      return ('Converged', k, time.process_time() - start_time, g_norm)

    ls = line_search(f, grad_f, x_k, d_k, gfk=g_k, c1=1e-4, c2=0.9)
    alpha_k = ls[0] if (ls is not None and ls[0] is not None) else 1e-4

    x_next = x_k + alpha_k * d_k
    g_next = grad_f(x_next)
    y_k = g_next - g_k

    dk_yk = np.dot(d_k, y_k)
    if abs(dk_yk) < 1e-18:
      dk_yk = 1e-18

    beta = 0.0
    if method == 'DY':
      beta = np.dot(g_next, g_next) / dk_yk

    elif method == 'HS':
      beta = np.dot(g_next, y_k) / dk_yk

    elif method == 'HZ':
      term1 = np.dot(
        y_k - 2 * d_k * (np.linalg.norm(y_k) ** 2 / dk_yk), g_next / dk_yk
      )
      beta = term1

    # Обновление направления
    d_k = -g_next + beta * d_k

    # Условие перезапуска
    if abs(np.dot(g_next, g_k)) >= 0.2 * np.dot(g_next, g_next):
      d_k = -g_next

    g_k = g_next
    x_k = x_next

  return ('Max Iter', max_iter, time.process_time() - start_time, g_norm)


funcs = [
  (
    'cosine',
    cosine_func,
    cosine_grad,
    lambda n: np.array([1.0 / i for i in range(1, n + 1)]),
  ),
  (
    'dixmaana',
    dixmaana,
    dixmaana_grad,
    lambda n: np.array([1.0 / i for i in range(1, n + 1)]),
  ),
  (
    'raydan2',
    raydan2,
    raydan2_grad,
    lambda n: np.array([1.0 / i for i in range(1, n + 1)]),
  ),
  (
    'genquartic',
    genquartic,
    genquartic_grad,
    lambda n: np.array([1.0 / i for i in range(1, n + 1)]),
  ),
  ('exdenschnf', exdenschnf, exdenschnf_grad, lambda n: np.full(n, 0.0)),
  (
    'liarwhd',
    liarwhd,
    liarwhd_grad,
    lambda n: np.array([1.0 / i for i in range(1, n + 1)]),
  ),
  (
    'himmelbg',
    himmelbg,
    himmelbg_grad,
    lambda n: np.array([1.0 / i for i in range(1, n + 1)]),
  ),
  (
    'powell',
    powell,
    powell_grad,
    lambda n: np.tile([3.0, -1.0, 0.0, 1.0], n // 4),
  ),
]

methods = ['hSSH1', 'DY', 'HS', 'HZ']
sizes = (150, 15000, 150000, 500000)

results = {s: {m: [] for m in methods} for s in sizes}
func_names = [f[0] for f in funcs]

print('size func method time g_norm')
for n in sizes:
  for name, f, grad, x0_gen in funcs:
    x0 = x0_gen(n)
    for m in methods:
      if m == 'hSSH1':
        _, iters, elapsed, g_final = hssh1_algo(f, grad, x0)
      else:
        _, iters, elapsed, g_final = run_cg_comparison(f, grad, x0, method=m)

      print(f'{n} {name} {m} {elapsed} {g_final}')
      results[n][m].append(iters)


fig, axes = plt.subplots(2, 2, figsize=(15, 10), constrained_layout=True)
axes_flat = axes.flatten()

for i, n in enumerate(sizes):
  ax = axes_flat[i]
  x_indices = np.arange(len(func_names))

  for j, m in enumerate(methods):

    # Добавляем смещение j * 0.1, чтобы линии стояли рядом
    ax.plot(
      x_indices + (j * 0.05), results[n][m], marker='o', label=m, alpha=0.8
    )

  ax.set_title(f'Размерность N = {n}')
  ax.set_ylabel('Количество итераций')
  ax.set_xticks(x_indices)
  ax.set_xticklabels(func_names, rotation=45)

  ax.grid(True, alpha=0.3)
  ax.legend()

plt.show()
