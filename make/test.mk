$(TESTS): $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lcriterion $(LDFLAGS)

test: $(TESTS)
	LD_LIBRARY_PATH=$(MPICH_TARGET_DIR)/lib ./$(TESTS)

