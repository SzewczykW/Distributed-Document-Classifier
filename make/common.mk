MPICH_TARGET_DIR ?= $(shell echo $$MPICH_TARGET_DIR)
ifeq ($(MPICH_TARGET_DIR),)
$(error MPICH_TARGET_DIR is not set. Please export it before running make.)
endif

GASPI_TARGET_DIR ?= $(shell echo $$GASPI_TARGET_DIR)
ifeq ($(GASPI_TARGET_DIR),)
$(error GASPI_TARGET_DIR is not set. Please export it before running make.)
endif

CC := $(MPICH_TARGET_DIR)/bin/mpicc

MPI_CFLAGS := -I$(MPICH_TARGET_DIR)/include
MPI_LDFLAGS := -L$(MPICH_TARGET_DIR)/lib -lmpi -Wl,-rpath=$(MPICH_TARGET_DIR)/lib

GASPI_CFLAGS := -I$(GASPI_TARGET_DIR)/include
GASPI_LDFLAGS := -L$(GASPI_TARGET_DIR)/lib64 -lgaspi -Wl,-rpath=$(GASPI_TARGET_DIR)/lib64

CFLAGS := -std=c17 -Wall -Wextra -Werror -Wpedantic -O3 -D_POSIX_C_SOURCE=200809L -Iinclude $(MPI_CFLAGS) $(GASPI_CFLAGS)
LDFLAGS := $(MPI_LDFLAGS) $(GASPI_LDFLAGS)

TEST_CFLAGS := -std=c17 -Wall -Wextra -Werror -Wpedantic -O3 -D_POSIX_C_SOURCE=200809L -Iinclude -Itests/include $(MPI_CFLAGS) $(GASPI_CFLAGS)
TEST_LDFLAGS := -lcriterion $(MPI_LDFLAGS) $(GASPI_LDFLAGS)

BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
TEST_OBJ_DIR := $(BUILD_DIR)/obj_test
DOCS_DIR := docs

COMMON_SRC := \
    src/hash_table.c    \
    src/file_utils.c    \
    src/classifier.c

SRC := \
    ${COMMON_SRC}       \
    src/main.c          \
    src/manager.c       \
    src/worker.c

TEST_SRC := \
    ${COMMON_SRC}               \
    tests/src/test_classifier.c \
    tests/src/test_file_utils.c \
    tests/src/test_hash_table.c \
    tests/src/utils.c

SRCS := $(SRC)
TEST_SRCS := $(TEST_SRC)
OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:%.c=$(TEST_OBJ_DIR)/%.o)

TARGET := $(BIN_DIR)/ddc
TESTS := $(BIN_DIR)/ddc-tests

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DOCS_DIR)/html
	rm -rf $(DOCS_DIR)/latex
