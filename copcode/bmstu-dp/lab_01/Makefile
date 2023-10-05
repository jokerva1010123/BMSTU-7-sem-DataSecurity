CC := gcc

LIB_NAME := libnumbers.a

SRC_DIR := ./src
OBJ_DIR := ./out

.PHONY: library clean

library:
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $(SRC_DIR)/numbers.c -o $(OBJ_DIR)/numbers.o
	ar rc $(LIB_NAME) $(OBJ_DIR)/numbers.o

clean:
	$(RM) -rf $(OBJ_DIR) *.exe *.a
