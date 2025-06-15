GASPI_FLAGS ?= -m nodes -n 16
RUN_FLAGS ?= ./example/input/ ./example/dict.txt ./example/output/result.txt

run: $(TARGET)
	$(GASPI_TARGET_DIR)/bin/gaspi_run $(GASPI_FLAGS) $(SCRIPTS_DIR)/gaspi_wrapper.sh $(TARGET) $(RUN_FLAGS)

run-help:
	@echo ""
	@echo "make run:"
	@echo "  Run the program using mpirun"
	@echo "  You can pass additional flags:"
	@echo "    GASPI_FLAGS='-f nodes -n 16'"
	@echo "    RUN_FLAGS='./example/input/ ./example/dict.txt ./example/output/result.txt'"
	@echo "  Example:"
	@echo "    make run GASPI_FLAGS='-f nodes -n 8' RUN_FLAGS='in_dir dict.txt out.txt'"
	@echo ""

