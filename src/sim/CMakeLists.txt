cmake_minimum_required(VERSION 3.8)  # Set the minimum CMake version
project(simlinx)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)           # Use C++17, change to 11, 14, 20, etc. as needed
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Ensure the compiler supports this standard
set(CMAKE_CXX_EXTENSIONS OFF)        # Use only standard-compliant C++

set(CMAKE_CXX_FLAGS "-g -fsanitize=address,undefined")

# Use LIEF with 'find_package()'
# ==============================

# Find LIEF. If LIEF was not installed into a default system directory then
# specify the following option during CMake configuration:
# -DLIEF_DIR=<LIEF install prefix>/share/LIEF/cmake
find_package(LIEF REQUIRED COMPONENTS STATIC) # COMPONENTS: <SHARED | STATIC> - Default: STATIC

set(INCLUDE_DIRECTORIES ../../include/)
set(SOURCES main.cc ../cpu/core.cc ../cpu/decodeTree.gen.cc ../cpu/execute.gen.cc)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE LIEF::LIEF)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})