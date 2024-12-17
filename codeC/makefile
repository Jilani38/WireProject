CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
CODEC_DIR = codeC
STATION_DIR = ..
OUTPUT_DIR = ./bin
SRC = $(CODEC_DIR)/main.c \
      $(CODEC_DIR)/avltree_usage.c \
      $(CODEC_DIR)/avltree_utils.c \
      $(CODEC_DIR)/utils.c \
      $(STATION_DIR)/station.c

OBJ = $(SRC:.c=.o)
TARGET = $(OUTPUT_DIR)/programme

all: prepare $(TARGET)
prepare:
	mkdir -p $(OUTPUT_DIR)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(OBJ) $(TARGET) $(OUTPUT_DIR)
mrproper: clean
	rm -rf $(OUTPUT_DIR)
