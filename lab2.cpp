#include "matrix.h"

#include <omp.h>

std::chrono::steady_clock::time_point start, end;


Matrix mul_with_OMP(const Matrix& first, const Matrix& second)
{
	start = std::chrono::high_resolution_clock::now();
	if (first.cols() != second.rows()) {
		throw std::invalid_argument("The number of columns of matrix A does not match the number of rows of matrix B");
	}
	double start = omp_get_wtime();
	Matrix result(first.rows(), second.cols());
	int i = 0, j = 0, k = 0;
	//omp_set_num_threads(12);
	std::cout << omp_get_num_threads() << " ";
#pragma omp parallel for shared(first, second, result) private(int i, int j, int k)
	for (i = 0; i < second.rows(); i++)
	{
		for (j = 0; j < first.cols(); j++)
		{
			result.at(i, j) = 0;
			for (k = 0; k < first.cols(); k++)
			{
				result.at(i, j) += first.at(i, k) * second.at(k, j);
			}

		}
	}
	std::cout << "time: " << omp_get_wtime() - start << std::endl;
	end = std::chrono::high_resolution_clock::now();
	return result;
}

Matrix load_from_file(const std::string& filename) {
	std::ifstream file(filename);
	if (file.is_open()) {
		int rows, cols;
		Matrix matrix(rows, cols);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				file >> matrix.at(i, j);
			}
		}
		file.close();
		return matrix;
	}
	else {
		throw std::invalid_argument("Can't load from file");
		return Matrix(0, 0);
	}
}

void experiments_lab2(const std::vector<size_t>& sizes) {
	std::ofstream file("../../../lab2/results_lab2.txt");
	if (!file.is_open())
		throw std::invalid_argument("Can't open file results_lab2.txt");
	for (size_t size : sizes) {
		Matrix A(size, size, 1);
		Matrix B(size, size, 1);
		A.save_to_file_lab2("A");
		B.save_to_file_lab2("B");
		mul_with_OMP(A, B).save_to_file_lab2("multiplyed");
		file << "size: " << size << ", time: " << std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() << std::endl;
	}
	file.close();
}

int main()
{
	srand(time(NULL));
	std::vector<size_t> sizes = {10, 100, 200, 500, 800, 1000, 1500, 2000};
	experiments_lab2(sizes);
	return 0;
}
