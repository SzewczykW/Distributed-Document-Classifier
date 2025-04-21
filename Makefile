# Ustawiamy katalog główny
ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
include $(ROOT)/make/common.mk
include $(ROOT)/make/build.mk
include $(ROOT)/make/run.mk
include $(ROOT)/make/test.mk
include $(ROOT)/make/docs.mk

.DEFAULT_GOAL := help

help:
	@echo ""
	@echo "Available targets:"
	@echo "  make build           - Compile the project into ./build/"
	@echo "  make run             - Run the program with mpirun (see 'make run-help')"
	@echo "  make test            - Build and run unit tests"
	@echo "  make docs            - Generate HTML and PDF documentation"
	@echo "  make clean           - Remove all build artifacts"
	@echo ""

