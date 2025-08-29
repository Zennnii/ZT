CC = gcc
CFLAGS = -Wall -Wextra -O2 -no-pie
OBJDIR = build
TARGET = zt

SRC = zt.c parser/parser.c utils/utils.c codegen/codegen.c
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c into .o inside build dir
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
