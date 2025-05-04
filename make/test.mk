test: $(TESTS)
	LD_LIBRARY_PATH=$(MPICH_TARGET_DIR)/lib ./$(TESTS)

$(TESTS): $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -Itests/include -o $@ $^ -lcriterion $(LDFLAGS)

$(TEST_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Itests/include -c $< -o $@
