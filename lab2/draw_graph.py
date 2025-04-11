import os
import re
import numpy as np
import matplotlib.pyplot as plt

data_file = "lab2/results_lab2.txt"

def read_data(file_path):
    data = {}
    current_size = None
    current_thread_count = None
    with open(file_path, 'r') as file:
        for line in file:
            if line.startswith("size:"):
                current_size = int(line.split(":")[1].strip())
                data[current_size] = {}
            elif line.startswith("\tthreads:"):
                parts = line.strip().split(": ")
                current_thread_count = int(parts[1].split(",")[0])
                times_str = parts[2].split(",")
                times = [float(t) for t in times_str if t]
                data[current_size][current_thread_count] = times
    return data

def calculate_stats(data):
    stats = {}
    for size, thread_data in data.items():
        stats[size] = {}
        for threads, times in thread_data.items():
            stats[size][threads] = {
                "mean": np.mean(times),
                "min": min(times),
                "max": max(times)
            }
    return stats

def plot_results(stats, filename):
    plt.figure(figsize=(12, 8))
    
    # Собираем все уникальные количества потоков
    thread_counts = set()
    for size in stats.values():
        thread_counts.update(size.keys())
    thread_counts = sorted(thread_counts)
    
    # Для каждого количества потоков строим график
    for threads in thread_counts:
        x = []
        y = []
        for size in sorted(stats.keys()):
            if threads in stats[size]:
                x.append(size)
                y.append(stats[size][threads]["mean"])
        plt.plot(x, y, 'o-', label=f"Threads: {threads}")
    
    plt.xlabel("Matrix Size")
    plt.ylabel("Average Time (s)")
    plt.title("Matrix Multiplication Performance")
    plt.legend()
    plt.grid(True)
    plt.savefig(filename.replace('\\', '/'))  # Исправляем путь для избежания экранирования
    plt.close()

# Основная часть
data = read_data(data_file)
stats = calculate_stats(data)

# Убедимся, что 1500 существует перед удалением
if 1500 in stats:
    plot_results(stats, "lab2/matrix_multiplication_performance.png")
    del stats[1500]
    plot_results(stats, "lab2/mmp_without_1500.png")
else:
    print("Warning: Size 1500 not found in data")
    plot_results(stats, "lab2/matrix_multiplication_performance.png")