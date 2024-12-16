import matplotlib.pyplot as plt
import numpy as np

# Читаем данные из файла
with open("output.txt", "r") as file:
    data = file.read()

# Разбиваем данные на массивы
data_lists = data.split("\n")  # Разделяем на строки
measurements = [list(map(int, line.split(",")[:-1])) for line in data_lists if line.strip()]  # Преобразуем строки в списки чисел

# Проверяем, что данные корректно загружены
measurements1, measurements2, measurements3 = measurements

# Генерация значений на оси X (индексы точек и размеры массива)
n_values = np.arange(len(measurements1))  # Количество точек по X
array_sizes = 256 * (1.1 ** n_values)  # Размер массива в байтах

# Перевод в Кб/Мб для подписей
array_sizes_kb = array_sizes / 256  # Перевод в Кб
array_sizes_labels = [f"{x:.1f} Кб" if x < 1024 else f"{x/1024:.2f} Мб" for x in array_sizes_kb]

# Построение графиков
plt.figure(figsize=(10, 6))

plt.plot(n_values, measurements1, label='последовательный', color='blue')
plt.plot(n_values, measurements2, label='обратный', color='green')
plt.plot(n_values, measurements3, label='случайный', color='red')

plt.title("Графики измерений")
plt.xlabel("размер массива")
plt.ylabel("количество тактов процессора")
plt.legend()

# Устанавливаем подписи на оси X без изменения масштаба
num_xticks = 10  # Количество подписей на оси X
xticks_indices = np.linspace(0, len(n_values) - 1, num_xticks, dtype=int)
plt.xticks(xticks_indices, [array_sizes_labels[i] for i in xticks_indices], rotation=45)

plt.grid(True)
plt.tight_layout()
plt.show()
