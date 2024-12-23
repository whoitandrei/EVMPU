import matplotlib.pyplot as plt
import numpy as np

# Читаем данные из файла
with open("output.txt", "r") as file:
    data = file.readlines()

# Разбиваем данные на массивы, извлекая только вторые значения из каждой строки
measurements = [int(line.split()[1]) for line in data if line.strip()]

# Генерация значений на оси X (индексы точек)
n_values = np.arange(len(measurements))  # Количество точек по X

# Перевод в Кб/Мб для подписей
array_sizes_labels = [x for x in range(1, len(measurements) + 1)]

# Построение графика
plt.figure(figsize=(10, 6))
plt.plot(n_values, measurements, color='blue')

plt.title("Графики измерений")
plt.xlabel("Количество фрагментов")
plt.ylabel("Количество тактов процессора")

# Устанавливаем подписи на оси X
num_xticks = 10  # Количество подписей на оси X
xticks_indices = np.linspace(0, len(n_values) - 1, num_xticks, dtype=int)
plt.xticks(xticks_indices, [array_sizes_labels[i] for i in xticks_indices], rotation=45)

plt.grid(True)
plt.tight_layout()
plt.show()
