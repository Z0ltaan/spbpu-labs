#!/usr/bin/env python3

import numpy as np
import time
from scipy.optimize import line_search


def _beta_DY(g_next, y, d):
  return (np.linalg.norm(g_next) ** 2) / np.dot(d, y)


def _beta_HS(g_next, y, d):
  return np.dot(g_next, y) / np.dot(d, y)


def _beta_HZ(g_next, y, d):
  dy = np.dot(d, y)
  term1 = np.dot(g_next, y) / dy
  term2 = 2 * (np.linalg.norm(y) ** 2 / dy) * (np.dot(g_next, d) / dy)
  return term1 - term2


def hssh1_algo(f, grad_f, x0, tol=1e-6, max_iter=2000, rho=0.001, sigma=0.9):
  start_time = time.process_time()

  # NOTE: Step 1
  x_k = x0.astype(np.float64)
  g_k = grad_f(x_k)
  d_k = -g_k

  gamma_k = 0.5  # Фиксировано для hSSH1

  for k in range(max_iter):
    # NOTE: Step 2
    if np.linalg.norm(g_k, ord=np.inf) < tol:
      return (
        'Converged',
        k,
        time.process_time() - start_time,
        np.linalg.norm(g_k, ord=np.inf),
      )

    # NOTE: Step 3
    ls_res = line_search(f, grad_f, x_k, d_k, gfk=g_k, c1=rho, c2=sigma)
    alpha_k = ls_res[0]

    if alpha_k is None:  # NOTE: Если поиск не сошелся, пробуем cвой малый шаг
      alpha_k = 1e-4

    # NOTE: Step 4
    x_k_next = x_k + alpha_k * d_k
    g_k_next = grad_f(x_k_next)
    y_k = g_k_next - g_k

    # NOTE: Step 5 Computing delta_k because gamma is fixed
    num = 2 * (np.linalg.norm(y_k) ** 2) * np.dot(g_k_next, d_k) * (gamma_k + 1)
    den = -np.dot(g_k_next, g_k) * np.dot(d_k, y_k) - 2 * (
      np.linalg.norm(y_k) ** 2
    ) * np.dot(g_k_next, d_k)
    delta_k = 1 - num / den if abs(den) > 1e-18 else 0.5

    # ПРОЕКЦИЯ ПАРАМЕТРОВ (согласно Proof 1)
    if delta_k < 0:
      delta_k = 0
    if delta_k > 1:
      delta_k = 1
    if delta_k + gamma_k >= 1:
      delta_k = 1 - gamma_k

    # NOTE: Step 6
    beta_h = (
      delta_k * _beta_DY(g_k_next, y_k, d_k)
      + gamma_k * _beta_HS(g_k_next, y_k, d_k)
      + (1 - delta_k - gamma_k) * _beta_HZ(g_k_next, y_k, d_k)
    )

    # NOTE: Step 7
    d_k_next = -g_k_next + beta_h * d_k
    if abs(np.dot(g_k_next, g_k)) >= 0.2 * (np.linalg.norm(g_k_next) ** 2):
      d_k = -g_k_next
    else:
      d_k = d_k_next

    g_k = g_k_next
    x_k = x_k_next

    # NOTE: Step 8 runs automaticaly as we are in a loop

  return (
    'Max Iter Reached',
    max_iter,
    time.process_time() - start_time,
    np.linalg.norm(g_k, ord=np.inf),
  )
