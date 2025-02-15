#include "matrix.h"

#include <omp.h>

double start, end;

Matrix mul_with_OMP(const Matrix& first, const Matrix& second)
{
	start = omp_get_wtime();
	if (first.cols() != second.rows()) {
		throw std::invalid_argument("The number of columns of matrix A does not match the number of rows of matrix B");
	}
	double start = omp_get_wtime();
	Matrix result(first.rows(), second.cols());
	int i = 0, j = 0, k = 0;
#pragma omp parallel  shared(first, second, result) private( i,  j,  k)
	{
#pragma omp for 
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
	}
	std::cout << "time: " << omp_get_wtime() - start << std::endl;
	end = omp_get_wtime();
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

void experiments_lab2(const std::vector<size_t>& sizes, const std::vector<size_t>& threads) {

	std::ofstream file("../../../lab2/results_lab2.txt");
	if (!file.is_open())
		throw std::invalid_argument("Can't open file results_lab2.txt");
	for (size_t size : sizes) {
		std::vector<size_t> times;
		Matrix A(size, size, 1);
		Matrix B(size, size, 1);
		A.save_to_file_lab2("A");
		B.save_to_file_lab2("B");
		file << "size: " << size;
		for (size_t thread : threads) {
			omp_set_num_threads(thread);
			file << "\n\tthreads: " << thread << ", times: ";
			for (size_t i = 0; i < 10; ++i) {
				mul_with_OMP(A, B).save_to_file_lab2("multiplyed", thread);
				 file << end - start << ", ";
			}
		}
		file << std::endl;
	}
	file.close();
}

int main()
{
	srand(time(NULL));
	std::vector<size_t> sizes = {10, 100, 200, 500, 800, 1000, 1500};
	std::vector<size_t> threads = { 1, 2, 4, 6, 8, 10, 12, 14, 16 };
	experiments_lab2(sizes, threads);
	return 0;
}
