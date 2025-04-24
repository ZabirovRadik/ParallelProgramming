import os
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict

def read_data_from_files():
    data_by_processes = defaultdict(list)
    for i in [1, 2, 4, 6, 8, 10, 12]:
        filename = f'lab3/data/results_times_{i}.txt'
        with open(filename, 'r') as file:
            lines = file.readlines()
            for line in lines[1:]:  # пропускаем заголовок
                size, processes, time = map(float, line.strip().split(','))
                data_by_processes[int(processes)].append((int(size), time))
    return data_by_processes

def plot_matrix_size_vs_time(data_by_processes):
    plt.figure(figsize=(10, 6))

    for processes, data in sorted(data_by_processes.items()):
        data.sort()  # сортируем по размеру матрицы
        sizes = [d[0] for d in data]
        times = [d[1] for d in data]
        plt.plot(sizes, times, marker='o', label=f'{processes} процессов')

    plt.xlabel('Размер матрицы (N)')
    plt.ylabel('Время (секунды)')
    plt.title('Зависимость времени от размера матрицы')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('lab3/graphic.png')
    plt.show()

# Основной код
data_by_processes = read_data_from_files()
plot_matrix_size_vs_time(data_by_processes)
