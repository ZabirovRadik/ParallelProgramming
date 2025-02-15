#include "matrix.h"

std::chrono::steady_clock::time_point start, end;


Matrix multiply(const Matrix& first, const Matrix& second) {
	start = std::chrono::high_resolution_clock::now();
	if (first.cols() != second.rows()) {
		throw std::invalid_argument("The number of columns of matrix A does not match the number of rows of matrix B");
	}

	Matrix result(first.rows(), second.cols());
	for (int i = 0; i < first.rows(); i++) {
		for (int j = 0; j < second.cols(); j++) {
			for (int k = 0; k < first.cols(); k++) {
				result.at(i, j) += first.at(i, k) * second.at(k, j);
			}
		}
	}
	end = std::chrono::high_resolution_clock::now();
	return result;
}

void experiments_lab1(const std::vector<size_t>& sizes) {
	std::ofstream file("../../../lab1/results_lab1.txt");
	if (!file.is_open())
		throw std::invalid_argument("Can't open file results_lab1.txt");
	for (size_t size : sizes) {
		Matrix A(size, size, 1);
		Matrix B(size, size, 1);
		A.save_to_file_lab1("A");
		B.save_to_file_lab1("B");
		multiply(A, B).save_to_file_lab1("multiplyed");
		file << "size: " << size << ", time: " << std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() << std::endl;
	}
	file.close();
}

int main()
{
	srand(time(NULL));
	std::vector<size_t> sizes = { 10, 100, 200, 500, 800, 1000, 1500, 2000 };
	experiments_lab1(sizes);
	return 0;
}