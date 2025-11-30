CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude -pthread
LDFLAGS = -pthread -lrt

SRC_DIR   = src
BUILD_DIR = build
BIN_DIR   = bin

SRCS = $(SRC_DIR)/server.c \
       $(SRC_DIR)/client.c \
       $(SRC_DIR)/main.c

OBJS   = $(BUILD_DIR)/server.o \
         $(BUILD_DIR)/client.o \
         $(BUILD_DIR)/main.o

TARGET = $(BIN_DIR)/manutencao

# alvo padrão
all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run-servidor: $(TARGET)
	$(TARGET) servidor

run-cliente: $(TARGET)
	$(TARGET) cliente 1 5

.PHONY: all clean run-servidor run-cliente