# Use LIEF with 'find_package()'
# ==============================

# Find LIEF. If LIEF was not installed into a default system directory then
# specify the following option during CMake configuration:
# -DLIEF_DIR=<LIEF install prefix>/share/LIEF/cmake
find_package(LIEF REQUIRED COMPONENTS STATIC) # COMPONENTS: <SHARED | STATIC> - Default: STATIC

# Add our executable
# ==================
add_executable(HelloLIEF helloTest.cpp)
add_executable(ElFReaderLIEF elfReaderTest.cpp)

# Link the executable with LIEF
target_link_libraries(HelloLIEF PRIVATE LIEF::LIEF)
target_link_libraries(ElFReaderLIEF PRIVATE LIEF::LIEF)

# Test 1
set(INCLUDE_DIRECTORIES ../../../include/)
set(SOURCES parserTest.cpp ../parser.cpp)

add_executable(ParserLIEF ${SOURCES})
target_link_libraries(ParserLIEF PRIVATE LIEF::LIEF)
target_include_directories(ParserLIEF PRIVATE ${INCLUDE_DIRECTORIES})