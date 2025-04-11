import re
from collections import defaultdict

def calculate_averages(input_file, output_file):
    with open(input_file, 'r') as f_in, open(output_file, 'w') as f_out:
        current_size = None
        
        for line in f_in:
            # Обрабатываем строку с размером матрицы
            if line.startswith('size:'):
                current_size = line.strip()
                f_out.write(line)
            
            # Обрабатываем строку с данными о потоках
            elif line.strip().startswith('threads:'):
                # Извлекаем количество потоков
                threads_part = re.search(r'threads: (\d+)', line).group(0)
                
                # Извлекаем все значения времени
                times = list(map(float, re.findall(r'\d+\.\d+', line.split('times:')[1])))
                avg_time = sum(times) / len(times) if times else 0
                
                # Формируем новую строку с средним значением
                new_line = f"\t{threads_part}, times: {avg_time:.7f},\n"
                f_out.write(new_line)
            
            # Просто копируем другие строки (если есть)
            else:
                f_out.write(line)

# Пример использования
if __name__ == "__main__":
    input_filename = "results_lab2.txt"
    output_filename = "results_lab2_avg.txt"
    
    calculate_averages(input_filename, output_filename)
    print(f"Файл с средними значениями сохранен как: {output_filename}")