MPI_FLAGS ?= -np 4
RUN_FLAGS ?= --input ./data --output result.txt

run: $(TARGET)
	LD_LIBRARY_PATH=$(MPICH_TARGET_DIR)/lib \
	mpirun $(MPI_FLAGS) $(TARGET) $(RUN_FLAGS)

run-help:
	@echo ""
	@echo "make run:"
	@echo "  Run the program using mpirun"
	@echo "  You can pass additional flags:"
	@echo "    MPI_FLAGS='-np 4'          (number of processes)"
	@echo "    RUN_FLAGS='--input ./data --output result.txt'"
	@echo "  Example:"
	@echo "    make run MPI_FLAGS='-np 8' RUN_FLAGS='--input in --output out.txt'"
	@echo ""

