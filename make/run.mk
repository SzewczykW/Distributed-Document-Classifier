MPI_FLAGS ?= -f nodes -n 16
RUN_FLAGS ?= --input ./example/input/ --output ./example/output/result.txt

run: $(TARGET)
	LD_LIBRARY_PATH=$(MPICH_TARGET_DIR)/lib MPIR_CVAR_ENABLE_GPU=0 mpiexec $(MPI_FLAGS) $(TARGET) $(RUN_FLAGS)

run-help:
	@echo ""
	@echo "make run:"
	@echo "  Run the program using mpirun"
	@echo "  You can pass additional flags:"
	@echo "    MPI_FLAGS='-f nodes -n 16'"
	@echo "    RUN_FLAGS='--input ./example/input/ --output ./example/output/result.txt'"
	@echo "  Example:"
	@echo "    make run MPI_FLAGS='-f nodes -n 8' RUN_FLAGS='--input in_dir --output out.txt'"
	@echo ""

