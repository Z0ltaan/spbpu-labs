#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.utils import shuffle
from PIL import Image

image = Image.open(
    "./venice-landmark-burano-island-canal-colorful-houses-boats-italy-long-exposure-photography-33513782.webp"
).convert("RGB")
image_np = np.array(image, dtype=np.float64) / 255.0  # Нормализация в [0, 1]

w, h, d = image_np.shape
image_array = np.reshape(image_np, (w * h, d))

plt.figure(figsize=(10, 6))
plt.title("Исходное изображение")
plt.imshow(image_np)
plt.axis("off")
plt.tight_layout()
plt.show()

n_colors = [32, 16, 8, 4, 2]
image_array_sample = shuffle(image_array, random_state=0, n_samples=1000)

for color_count in n_colors:
    kmeans = KMeans(n_clusters=color_count, random_state=0, n_init=10).fit(
        image_array_sample
    )

    labels = kmeans.predict(image_array)
    new_palette = kmeans.cluster_centers_
    compressed_image_array = new_palette[labels]
    compressed_image = np.reshape(compressed_image_array, (w, h, d))

    plt.figure(figsize=(10, 6))
    plt.title(f"Сжатое изображение ({color_count} кластеров / цветов)")
    plt.imshow(compressed_image)
    plt.axis("off")
    plt.tight_layout()
    plt.show()

# import numpy as np
# import matplotlib.pyplot as plt
# from sklearn.cluster import KMeans
# from sklearn.utils import shuffle
# import urllib.request
# from PIL import Image
#
# image = Image.open("bgi3_city_greengray_1920_1080.jpg").convert("RGB")
# image_np = np.array(image, dtype=np.float64) / 255.0
#
# w, h, d = image_np.shape
# image_array = np.reshape(image_np, (w * h, d))
#
# # 3. Настройка сжатия палитры
# n_colors = 8
# kmeans = KMeans(n_clusters=n_colors, random_state=42, n_init=5).fit(image_array)
#
# labels = kmeans.labels_
# new_palette = kmeans.cluster_centers_
# compressed_image_array = new_palette[labels]
# compressed_image = np.reshape(compressed_image_array, (w, h, d))
#
# # 4. Исправленный симметричный вывод без пустых полей
# fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 7), sharey=True)
#
# # Левый график: Исходное
# ax1.imshow(image_np)
# ax1.set_title("Исходное изображение", fontsize=14, pad=10)
# ax1.axis("off")
#
# # Правый график: Сжатое
# ax2.imshow(compressed_image)
# ax2.set_title(f"Сжатая палитра ({n_colors} цветов)", fontsize=14, pad=10)
# ax2.axis("off")
#
# # Ограничиваем размеры отображения строго по границам контента
# plt.subplots_adjust(left=0.01, right=0.99, top=0.95, bottom=0.05, wspace=0.05)
# plt.show()
#
# image = Image.open("bgi3_city_greengray_1920_1080.jpg").convert("RGB")
# image_np = np.array(image, dtype=np.float64) / 255.0  # Нормализация в [0, 1]
#
# w, h, d = original_shape = tuple(image_np.shape)
# image_array = np.reshape(image_np, (w * h, d))
#
# plt.figure(figsize=(16, 9))  # plt.figure(figsize=(12, 6))
#
# # Исходное изображение
# plt.subplot(1, 2, 1)
# plt.title("Исходное изображение")
# plt.imshow(image_np)
# plt.axis("off")
# plt.show()
#
# n_colors = [32, 16, 8, 4, 2]
#
# for color_count in n_colors:
#     # Обучение K-Means на случайной выборке пикселей (для ускорения работы)
#     image_array_sample = shuffle(image_array, random_state=0, n_samples=1000)
#     kmeans = KMeans(n_clusters=color_count, random_state=0, n_init=10).fit(
#         image_array_sample
#     )
#
#     # Получение центров кластеров (палитры) и замена цветов для всех пикселей
#     labels = kmeans.predict(image_array)
#     new_palette = kmeans.cluster_centers_
#     compressed_image_array = new_palette[labels]
#     compressed_image = np.reshape(compressed_image_array, (w, h, d))
#
#     plt.subplot(1, 2, 2)
#     plt.title(f"Сжатое изображение ({color_count} кластеров)")
#     plt.imshow(compressed_image)
#     plt.axis("off")
#
#     plt.tight_layout()
#     plt.show()
