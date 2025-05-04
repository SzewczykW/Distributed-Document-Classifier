cmake_minimum_required(VERSION 3.14)
project(document_classifier_mpi C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================
# === MPICH Path ==============
# =============================
if(NOT DEFINED ENV{MPICH_TARGET_DIR})
    message(FATAL_ERROR "MPICH_TARGET_DIR is not set. Please export it before running CMake.")
endif()

set(MPI_HOME $ENV{MPICH_TARGET_DIR})
set(CMAKE_C_COMPILER "${MPI_HOME}/bin/mpicc")
add_compile_definitions(_POSIX_C_SOURCE=200809L)
add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wpedantic
    -O3
)

# =============================
# === Main application ========
# =============================
add_executable(ddc
    src/classifier.c
    src/file_utils.c
    src/hash_table.c
    src/main.c
    src/manager.c
    src/worker.c
)

target_include_directories(ddc PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${MPI_HOME}/include"
)

target_link_directories(ddc PRIVATE
    "${MPI_HOME}/lib"
)

target_link_libraries(ddc PRIVATE
    mpi
)

# =============================
# === Tests ===================
# =============================
include(FetchContent)

FetchContent_Declare(
    criterion
    GIT_REPOSITORY https://github.com/Snaipe/Criterion.git
    GIT_TAG        v2.4.2
)

FetchContent_MakeAvailable(criterion)

add_executable(ddc-tests
    src/classifier.c
    src/file_utils.c
    src/hash_table.c
    tests/src/test_classifier.c
    tests/src/test_file_utils.c
    tests/src/test_hash_table.c
    tests/src/utils.c
)

target_include_directories(ddc-tests PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/tests/include"
)

target_link_libraries(ddc-tests PRIVATE
    criterion
)

enable_testing()
add_test(NAME ddc-tests COMMAND ddc-tests)

# =============================
# === Documentation (Doxygen) ==
# =============================
find_package(Doxygen REQUIRED)

set(DOXYFILE_IN "${CMAKE_SOURCE_DIR}/docs/Doxyfile")

add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM
)

