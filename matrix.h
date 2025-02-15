#include <iostream>
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
	return s.w % 1000;
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

	void save_to_file_lab1(const std::string& filename) const {

		if (!std::filesystem::exists("../../../lab1/matrix&muls/" + std::to_string(data.size())))
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
	void save_to_file_lab2(const std::string& filename) const {

		if (!std::filesystem::exists("../../../lab2/matrix&muls&results/" + std::to_string(data.size())))
			std::filesystem::create_directories("../../../lab2/matrix&muls&results/" + std::to_string(data.size()));
		std::ofstream file("../../../lab2/matrix&muls&results/" + std::to_string(data.size()) + "/" + filename + ".txt");
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