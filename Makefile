CC ?= cc
CFLAGS = -g -std=c99 -Wall -Werror=implicit-function-declaration -Wno-unused-variable -I ./libs/ -I ./include/ 
SRC = $(wildcard src/*.c) libs/cJSON/cJSON.c
OBJ = $(patsubst %.c,%.o,$(SRC))
TARGET = libmastodont.a # shared
AR = ar

all: static

docs: 
	doxygen Doxyfile

static: $(OBJ)
	$(AR) rcs $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)

.PHONY: clean docs all test
