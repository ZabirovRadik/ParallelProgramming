#include <mpi.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <filesystem>

size_t XorShift128() {
    static size_t x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    size_t t = x ^ (x << 11);
    x = y; y = z; z = w;
    w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    return w % 1000;
}

void save_result(const std::vector<std::vector<size_t>>& mat,
    const size_t num_threads,
    const std::string& filename)
{
    size_t N = mat.size();
    std::filesystem::path dir = "../lab3/data/" + std::to_string(num_threads ) + "/" + std::to_string(N);
    if (!exists(dir)) {
        create_directories(dir);
    }
    std::filesystem::path filepath = dir / (filename + ".txt");
    std::ofstream file(filepath);
    if (!file) {
        throw std::runtime_error("Can't open file for writing: " + filepath.string());
    }
    for (const auto& row : mat) {
        for (size_t v : row) {
            file << v << ' ';
        }
        file << "\n";
    }
}
void save_base_matrix(const std::vector<std::vector<size_t>>& mat,
    const std::string& filename) {
    size_t N = mat.size();
    std::filesystem::path dir = "../lab3/base/" + std::to_string(N);
    if (!exists(dir)) {
        create_directories(dir);
    }
    std::filesystem::path filepath = dir / (filename + ".txt");
    std::ofstream file(filepath);
    if (!file) {
        throw std::runtime_error("Can't open file for writing: " + filepath.string());
    }    for (const auto& row : mat) {
        for (size_t v : row) {
            file << v << ' ';
        }
        file << "\n";
    }
}
std::vector<std::vector<size_t>> load_base_from_file(size_t N, const std::string& filename) {
    std::filesystem::path filepath = "../lab3/base/"  + std::to_string(N) + "/" + filename + ".txt";
    std::ifstream file(filepath);
    if (!file) throw std::runtime_error("Cannot open file: " + filepath.string());

    std::vector mat(N, std::vector<size_t>(N));
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            file >> mat[i][j];

    return mat;
}


const std::vector<size_t> sizes = {50, 100, 200, 400, 600, 1000, 1400};

int main(int argc, char** argv) {

    bool use_existing = false;
    if (argc > 1 && std::string(argv[1]) == "load") use_existing = true;

    MPI_Init(&argc, &argv);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    std::ofstream log;
    if (world_rank == 0) {
        std::filesystem::path logdir = "../lab3/data";
        if (!exists(logdir)) {
            create_directories(logdir);
        }
        log.open(logdir / ("results_times_" + std::to_string(world_size) + ".txt"));
        log << "MatrixSize,Processes,TimeSeconds\n";
    }


    for (size_t N : sizes) {
        MPI_Barrier(MPI_COMM_WORLD);

        std::vector<std::vector<size_t>> A, B;
        if (world_rank == 0) {
            if (use_existing) {
                A = load_base_from_file(N, "A");
                B = load_base_from_file(N, "B");
            }
            else {
                A.assign(N, std::vector<size_t>(N));
                B.assign(N, std::vector<size_t>(N));
                for (size_t i = 0; i < N; ++i) {
                    for (size_t j = 0; j < N; ++j) {
                        A[i][j] = XorShift128();
                        B[i][j] = XorShift128();
                    }
                }
                save_base_matrix(A, "A");
                save_base_matrix(B, "B");
            }
        }

        if (world_rank != 0) {
            B.assign(N, std::vector<size_t>(N));
        }
        for (size_t i = 0; i < N; ++i) {
            MPI_Bcast(B[i].data(), static_cast<int>(N), MPI_UNSIGNED_LONG_LONG,
                      0, MPI_COMM_WORLD);
        }

        int base = static_cast<int>(N / world_size);
        int rem = static_cast<int>(N % world_size);
        std::vector<int> sendcounts(world_size), displs(world_size);
        int offset = 0;
        for (int r = 0; r < world_size; ++r) {
            int rows = base + (r < rem ? 1 : 0);
            sendcounts[r] = rows * static_cast<int>(N);
            displs[r]     = offset;
            offset       += sendcounts[r];
        }
        int my_rows = sendcounts[world_rank] / static_cast<int>(N);

        std::vector<size_t> flat_A;
        if (world_rank == 0) {
            flat_A.resize(N * N);
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < N; ++j)
                    flat_A[i * N + j] = A[i][j];
        }
        std::vector<size_t> flat_local_A(my_rows * N);
        MPI_Scatterv(
            flat_A.data(), sendcounts.data(), displs.data(), MPI_UNSIGNED_LONG_LONG,
            flat_local_A.data(), my_rows * N, MPI_UNSIGNED_LONG_LONG,
            0, MPI_COMM_WORLD
        );

        std::vector local_A(my_rows, std::vector<size_t>(N));
        for (int i = 0; i < my_rows; ++i)
            for (size_t j = 0; j < N; ++j)
                local_A[i][j] = flat_local_A[i * N + j];

        std::vector local_C(my_rows, std::vector(N, 0ULL));

        MPI_Barrier(MPI_COMM_WORLD);
        auto t_start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < my_rows; ++i) {
            for (size_t j = 0; j < N; ++j) {
                size_t sum = 0;
                for (size_t k = 0; k < N; ++k) {
                    sum += local_A[i][k] * B[k][j];
                }
                local_C[i][j] = sum;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        auto t_end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(t_end - t_start).count();

        std::vector<size_t> flat_local_C(my_rows * N);
        for (int i = 0; i < my_rows; ++i)
            for (size_t j = 0; j < N; ++j)
                flat_local_C[i * N + j] = local_C[i][j];

        std::vector<size_t> flat_C;
        if (world_rank == 0) {
            flat_C.resize(N * N);
        }
        MPI_Gatherv(
            flat_local_C.data(), my_rows * N, MPI_UNSIGNED_LONG_LONG,
            flat_C.data(), sendcounts.data(), displs.data(), MPI_UNSIGNED_LONG_LONG,
            0, MPI_COMM_WORLD
        );

        if (world_rank == 0) {
            std::vector C(N, std::vector<size_t>(N));
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < N; ++j)
                    C[i][j] = flat_C[i * N + j];

            log << "Size: " << N
                      << ", Processes: " << world_size
                      << ", Time: " << elapsed << " s\n";
            std::cout << "Size: " << N
                      << ", Processes: " << world_size
                      << ", Time: " << elapsed << " s\n";

            save_result(C,world_size, "result" );
        }
    }

    if (world_rank == 0) log.close();
    MPI_Finalize();
    return 0;
}
