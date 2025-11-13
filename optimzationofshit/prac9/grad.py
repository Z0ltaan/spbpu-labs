import numpy as np

# Матрица A и вектор b
A = np.array([[640.9, -237.6],
              [-237.6, 96.4]])
b = np.array([9.125, 9.125])

# Начальное приближение
x0 = np.array([-1, 0])

# Точность
tol = 1e-10
max_iter = 1000


def gradient_descent(A, b, x0, tol, max_iter):
    x = x0.copy()
    history = [x.copy()]

    for i in range(max_iter):
        r = A @ x - b  # градиент
        if np.linalg.norm(r) < tol:
            break
        # Оптимальный шаг
        alpha = np.dot(r, r) / np.dot(r, A @ r)
        x = x - alpha * r
        history.append(x.copy())

    return x, np.array(history)


# Запуск
x_opt, history = gradient_descent(A, b, x0, tol, max_iter)

print("Минимум находится в точке:", x_opt)
print("Значение функции в минимуме:", 0.5 * x_opt @ A @ x_opt - b @ x_opt)
print("Количество итераций:", len(history))

# Вывод траектории
print("\nТраектория спуска:")
for i, point in enumerate(history):
    print(f"Итерация {i}: ({point[0]:.10f}, {point[1]:.10f})")
