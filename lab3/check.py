import os
import numpy as np

def read_matrix_from_file(filepath):
    """Функция для чтения матрицы из файла"""
    with open(filepath, 'r') as file:
        lines = file.readlines()
        matrix = np.array([list(map(int, line.strip().split())) for line in lines])
    return matrix

def compare_matrices(base_dir, data_dir):
    """Функция для сравнения матриц из папок base и data"""
    with open('lab3/trust_me.txt', 'w') as trust_file:
        for folder in os.listdir(base_dir):
            if folder.isdigit():
                base_folder = os.path.join(base_dir, folder)
                a_file = os.path.join(base_folder, 'A.txt')
                b_file = os.path.join(base_folder, 'B.txt')
                a_matrix = read_matrix_from_file(a_file)
                b_matrix = read_matrix_from_file(b_file)
                result_matrix = np.dot(a_matrix, b_matrix)

                for folder_in_data_ in os.listdir(os.path.join(data_dir)):
                    if folder_in_data_.isdigit():
                        data_folder = os.path.join(data_dir, folder_in_data_, folder)
                        result_file = os.path.join(data_folder, 'result.txt')
                        expected_result = read_matrix_from_file(result_file)
                        if np.array_equal(result_matrix, expected_result):
                            trust_file.write(f"Матрицы совпадают при threads={folder_in_data_} и при размере {folder}!!!\n")
                        else:
                            trust_file.write(f"Матрицы НЕ совпадают при threads={folder_in_data_} и при размере {folder}\n")

# Вызов функции
compare_matrices('lab3/base', 'lab3/data')