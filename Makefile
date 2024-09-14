

SRC_DIR = $(shell pwd)
export BUILD_DIR ?= $(SRC_DIR)/build
export INCLUDE_DIR = $(SRC_DIR)/include

.PHONY: pmparser
pmparser:
	mkdir -p $(BUILD_DIR) && \
	$(CC) $(SRC_DIR)/$@.c -I$(INCLUDE_DIR) -c -o $(BUILD_DIR)/$@.o && \
	ar rcs $(BUILD_DIR)/lib$@.a $(BUILD_DIR)/$@.o


.PHONY: examples
examples: pmparser
	make -C $(SRC_DIR)/examples


CFLAGS+=-I $(INCLUDE_DIR)
CFLAGS+=-g
CFLAGS+=-Wall
CFLAGS+=-Wextra

# debería de incluirse en Wextra por lo menos
# https://stackoverflow.com/questions/22067562/how-c-handles-the-same-variable-name-along-different-scopes
# https://stackoverflow.com/questions/32254127/c-prevent-use-of-same-variable-name-in-block-scope
# https://gcc.gnu.org/onlinedocs/gcc-14.2.0/gcc/Warning-Options.html#index-Wshadow
CFLAGS+=-Wshadow

.PHONY: examples
test: test.o pmparser_mine.o
	$(CC) $(LDFLAGS) $^ -o $@
