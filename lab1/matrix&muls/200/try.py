import numpy as np

def read_matrix_from_file(filename):
    matrix = []
    with open(filename, 'r') as file:
        for line in file:
            row = [int(x) for x in line.strip().split()]
            matrix.append(row)
    return np.array(matrix)

# Чтение матриц из файлов
A = read_matrix_from_file('D:\Parallel\PP\lab1\matrix&muls/200/A.txt')
B = read_matrix_from_file('D:\Parallel\PP\lab1\matrix&muls/200/B.txt')
C = read_matrix_from_file('D:\Parallel\PP\lab1\matrix&muls/200/multyplyed.txt')

# Перемножение матриц A и B
result = A@B

# Сравнение результата с матрицей из файла multiplyed.txt
if np.array_equal(result, C):
    print("Результат перемножения матриц совпадает с матрицей из файла multiplyed.txt")
else:
    print("Результат перемножения матриц не совпадает с матрицей из файла multiplyed.txt")