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

# =============================
# === Main application ========
# =============================

add_executable(classifier
    src/main.c
    src/classifier.c
    src/utils.c
)

target_include_directories(classifier PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${MPI_HOME}/include"
)

target_link_directories(classifier PRIVATE
    "${MPI_HOME}/lib"
)

target_link_libraries(classifier PRIVATE mpi)

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

add_executable(tests
    tests/test_classification.c
)

target_include_directories(tests PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${MPI_HOME}/include"
)

target_link_directories(tests PRIVATE
    "${MPI_HOME}/lib"
)

target_link_libraries(tests PRIVATE
    criterion
    mpi
)

enable_testing()
add_test(NAME unit_tests COMMAND tests)

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

