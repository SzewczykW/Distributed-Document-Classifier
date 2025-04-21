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
DOCS_DIR := docs

SRC := src/main.c src/manager.c src/worker.c src/utils.c
CORE := core/profile.c
LIB := lib/hash_table.c lib/io_utils.c
TEST_SRC := tests/test_classification.c

SRCS := $(SRC) $(CORE) $(LIB)
OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)
TEST_OBJS := $(TEST_SRC:%.c=$(OBJ_DIR)/%.o)

TARGET := $(BIN_DIR)/ddc
TESTS := $(BIN_DIR)/ddc-tests

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DOCS_DIR)/html
	rm -rf $(DOCS_DIR)/latex
