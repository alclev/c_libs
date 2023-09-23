obj_dir := ./build/obj/
DEST := ./build/bin/
LIBS := $(wildcard ./libs/*.c)

CC := gcc
CFLAGS := -Wall -Wwrite-strings -Wextra -g
TARGET := exe
SRC := $(wildcard ./src/*.c)
OBJ := $(patsubst %.c, $(obj_dir)%.o, $(notdir $(SRC)))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ) | create_dirs
	@echo 'Linking...'
	@echo $(OBJ) '-->' $(DEST)$(TARGET)
	@$(CC) $(CFLAGS) -o $(DEST)$(TARGET) $(OBJ) $(LIBS)

$(obj_dir)%.o: ./src/%.c
	@echo $< '-->' $(DEST)$@
	@$(CC) $(CFLAGS) -c $< -o $@

create_dirs:
	@mkdir -p $(obj_dir)
	@mkdir -p $(DEST)

clean:
	@echo 'Cleaning up...'
	@rm -rf $(obj_dir)*.o $(DEST)$(TARGET)
