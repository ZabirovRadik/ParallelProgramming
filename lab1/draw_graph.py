import matplotlib.pyplot as plt

# Читаем данные из файла
with open("D:\Parallel\PP\lab1\\results_lab1.txt", "r") as file:
    data = file.readlines()

# Извлекаем размеры и времена выполнения
sizes = []
times = []
for line in data:
    if "size" in line and "time" in line:
        size, time = line.split(", ")
        size = float(size.split(": ")[1])
        time = float(time.split(": ")[1])
        sizes.append(size)
        times.append(time)

# Строим график
plt.figure(figsize=(10, 6))
plt.plot(sizes, times)
plt.title("Зависимость времени выполнения от размера матрицы")
plt.xlabel("Размер матрицы")
plt.ylabel("Время выполнения, секунды")
plt.grid(True)

# Сохраняем график в файл
plt.savefig("d:\Parallel\PP\lab1\chart.png")
plt.show()
