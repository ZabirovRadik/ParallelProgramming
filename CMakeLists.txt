cmake_minimum_required(VERSION 3.10)

# Enable hot reload for MSVC compilers if supported
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Find MPI (must be before adding executables)
find_package(MPI REQUIRED)

project("PP")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executables
add_executable(lab1 "lab1.cpp" "matrix.h")
add_executable(lab2 "lab2.cpp" "matrix.h")
add_executable(lab3 "lab3.cpp" "matrix.h")

# Find OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(lab2 PUBLIC OpenMP::OpenMP_CXX)
endif()

include_directories("C:/Program Files (x86)/Microsoft SDKs/MPI/Include")
link_directories("C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64")
# Add MPI support to all targets
# Явное указание путей
if(WIN32)
    set(MPI_LIBRARY "C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64/msmpi.lib")
    set(MPI_INCLUDE_DIR "C:/Program Files (x86)/Microsoft SDKs/MPI/Include")

    if(NOT EXISTS ${MPI_LIBRARY})
        message(FATAL_ERROR "MS-MPI library not found at ${MPI_LIBRARY}")
    endif()

    target_link_libraries(lab3 PRIVATE ${MPI_LIBRARY})
    target_include_directories(lab3 PRIVATE ${MPI_INCLUDE_DIR})
endif()