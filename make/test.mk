test: $(TESTS)
	./$(TESTS)

$(TESTS): $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(TEST_CFLAGS) -o $@ $^ $(TEST_LDFLAGS)

$(TEST_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -c $< -o $@
