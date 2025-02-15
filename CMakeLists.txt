# CMakeList.txt: проект CMake для PP; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.10)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_FLAG}")
project ("PP")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (lab1 "lab1.cpp" "matrix.h")
add_executable (lab2 "lab2.cpp" "matrix.h")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(lab2 PUBLIC OpenMP::OpenMP_CXX)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab1 lab2 PROPERTY CXX_STANDARD 20)
endif()
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
