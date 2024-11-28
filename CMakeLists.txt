cmake_minimum_required(VERSION 3.20)
project(simlinx)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Default build type" FORCE)
endif()

if (NOT DEFINED LIEF_DIR)
  if (NOT EXISTS lib/LIEF)
    execute_process(COMMAND bash installLIEF.sh)
  endif()
  set(LIEF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/LIEF/lib/cmake/LIEF)
endif()

execute_process(
  COMMAND python3 generate/generator.py
  COMMAND python3 generate/syscall/sysGen.py
)

include_directories(include)
add_subdirectory(src)