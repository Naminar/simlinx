list(APPEND SOURCES main.cc)
add_executable(${PROJECT_NAME} ${SOURCES})

set(LIEF_LIBRARY_TYPE SHARED)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
    set(LIEF_LIBRARY_TYPE STATIC)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

find_package(LIEF REQUIRED COMPONENTS ${LIEF_LIBRARY_TYPE})
target_link_libraries(${PROJECT_NAME} PRIVATE LIEF::LIEF)