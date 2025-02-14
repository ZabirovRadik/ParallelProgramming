import os
import numpy as np


def read_matrix_from_file(filename):
    matrix = []
    with open(filename, 'r') as file:
        for line in file:
            row = [int(x) for x in line.strip().split()]
            matrix.append(row)
    return np.array(matrix)


def process_folder(folder_path):
    A = read_matrix_from_file(os.path.join(folder_path, 'A.txt'))
    B = read_matrix_from_file(os.path.join(folder_path, 'B.txt'))
    C = read_matrix_from_file(os.path.join(folder_path, 'multiplyed.txt'))

    result = A @ B
    
    if np.array_equal(result, C):
        return f"Для {A.shape[1]} умножение посчитало правильно!\n"
    else:
        return f"В папке {A.shape[1]} результат перемножения матриц неправильный\n"


current_dir = os.path.dirname(os.path.abspath(__file__))
with open(os.path.join(current_dir, 'trust_me.txt'), 'w') as f:
    for folder in os.listdir(os.path.join(current_dir, "matrix&muls")):
        folder_path = os.path.join(current_dir, "matrix&muls", folder)
        if os.path.isdir(folder_path):
            f.write(process_folder(folder_path))