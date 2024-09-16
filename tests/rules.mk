TEST_SRC := $(wildcard tests/*.c)
TEST_OBJ := $(patsubst tests/%.c,$(BUILD_DIR)/tests/%.o,$(TEST_SRC))

MY_LIBS := $(BUILD_DIR)/lib$(STEM).a

$(BUILD_DIR)/bin/tests: $(TEST_OBJ) $(MY_LIBS) | tests/rules.mk
	@echo " LD " $@
	$(QUIET)$(CC) -o $@ $(LDFLAGS) $^ $(LIBS) $($(dir)_LIBS)

$(BUILD_DIR)/tests/%.o: tests/%.c $(MY_LIBS) | tests/rules.mk $(BUILD_DIR)/tests
	@echo " CC " $@
	$(QUIET)$(CC) -Itests $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/tests:
	@test -d $@ || \
		echo "MKDR" $@ && \
		mkdir -p $@
