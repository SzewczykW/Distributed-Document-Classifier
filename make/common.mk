MPICH_TARGET_DIR ?= $(shell echo $$MPICH_TARGET_DIR)
ifeq ($(MPICH_TARGET_DIR),)
$(error MPICH_TARGET_DIR is not set. Please export it before running make.)
endif

CC := $(MPICH_TARGET_DIR)/bin/mpicc
CFLAGS := -std=c17 -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -Iinclude -I$(MPICH_TARGET_DIR)/include
LDFLAGS := -L$(MPICH_TARGET_DIR)/lib -lmpi

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

SRCS := $(SRC) $(CORE) $(LIB)
OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)
TEST_OBJS := $(TEST_SRC:%.c=$(TEST_OBJ_DIR)/%.o)

TARGET := $(BIN_DIR)/ddc
TESTS := $(BIN_DIR)/ddc-tests

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DOCS_DIR)/html
	rm -rf $(DOCS_DIR)/latex
