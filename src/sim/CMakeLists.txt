# Find LIEF. If LIEF was not installed into a default system directory then
# specify the following option during CMake configuration:
# -DLIEF_DIR=<LIEF install prefix>/share/LIEF/cmake
find_package(LIEF REQUIRED COMPONENTS STATIC) # COMPONENTS: <SHARED | STATIC> - Default: STATIC

set(SOURCES main.cc ../cpu/core.cc ../cpu/decodeTree.gen.cc ../cpu/execute.gen.cc)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE LIEF::LIEF)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})