// PP.cpp: определяет точку входа для приложения.
//

#include "PP.h"
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <filesystem>


struct seed_t
{
	size_t x = rand();
	size_t y = rand();
	size_t z = rand();
	size_t w = rand();
};

size_t XorShift128()
{
	static seed_t s;
	size_t t = s.x ^ (s.x << 11);
	s.x = s.y;
	s.y = s.z;
	s.z = s.w;
	s.w = (s.w ^ (s.w >> 19)) ^ (t ^ (t >> 8));
	return s.w%1000;
}

class Matrix {
	std::vector<std::vector<size_t>> data;
public:
	Matrix(size_t rows, size_t cols) : data(cols, std::vector<size_t>(rows)) {
		for (size_t i = 0; i < cols; ++i) {
			for (size_t j = 0; j < rows; ++j) {
				data[i][j] = 0;
			}
		}
	}
	Matrix(size_t rows, size_t cols, int i) : data(cols, std::vector<size_t>(rows)) {
		for (size_t i = 0; i < cols; ++i) {
			for (size_t j = 0; j < rows; ++j) {
				data[i][j] = XorShift128();
			}
		}
	}
	size_t cols() const {
		return data.size();
	}
	size_t rows() const {
		return data[0].size();
	}
	size_t at(size_t col, size_t row) const {
		return data[col][row];
	}
	size_t& at(size_t col, size_t row) {
		return data[col][row];
	}
	void print() const {
		std::cout << "Matrix:" << std::endl;
		for (const auto& col : data) {
			for (const auto& value : col) {
				std::cout << value << " ";
			}
			std::cout << std::endl;
		}
	}

	void save_to_file(const std::string& filename) const {
		if(!std::filesystem::exists("../../../lab1/matrix&muls/" + std::to_string(data.size())))
			std::filesystem::create_directories("../../../lab1/matrix&muls/" + std::to_string(data.size()));
		std::ofstream file("../../../lab1/matrix&muls/" + std::to_string(data.size()) + "/" + filename + ".txt");
		if (file.is_open()) {
			for (const auto& col : data) {
				for (const auto& value : col) {
					file << value << " ";
				}
				file << std::endl;
			}
			file.close();
		}
		else {
			throw std::invalid_argument("Can't save to file");
		}
	}
};

Matrix multiply(const Matrix& first, const Matrix& second) {
	if (first.cols() != second.rows()) {
		throw std::invalid_argument("The number of columns of matrix A does not match the number of rows of matrix B");
	}

	Matrix result(first.rows(), second.cols());
	for (int i = 0; i < first.rows(); i++) {
		for (int j = 0; j < second.cols(); j++) {
			for (int k = 0; k < first.cols(); k++) {
				result.at(i,j) += first.at(i, k) * second.at(k, j);
			}
		}
	}
	return result;
}

Matrix load_from_file(const std::string& filename) {
	std::ifstream file(filename);
	if (file.is_open()) {
		int rows, cols;
		Matrix matrix(rows, cols);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				file >> matrix.at(i, j); // Считываем элементы матрицы
			}
		}
		file.close();
		return matrix;
	}
	else {
		throw std::invalid_argument("Can't load from file");
		return Matrix(0, 0); // Возвращаем пустую матрицу
	}
}

void experiments(const std::vector<size_t>& sizes) {
	std::ofstream file("../../../results_lab1.txt");
	if (!file.is_open())
		throw std::invalid_argument("Can't open file results_lab1.txt");
	for (size_t size : sizes) {
		Matrix A(size, size, 1);
		Matrix B(size, size, 1);
		A.save_to_file("A");
		B.save_to_file("B");
		auto start = std::chrono::high_resolution_clock::now();
		multiply(A, B).save_to_file("multyplyed");
		file << "size: " << size << ", time: " << std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;
	}
	file.close();
}

int main()
{
	srand(time(NULL));
	std::vector<size_t> sizes = {10, 100, 200, 500, 800, 1000, 1500, 2000};
	experiments(sizes);
	

	//multiply(matr, matr1).print();
	return 0;
}
