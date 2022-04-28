CC ?= cc
CFLAGS = -g -std=c99 -Wall -Werror=implicit-function-declaration -Wno-unused-variable -I ./include/ `pkg-config --cflags libcjson`
SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
TARGET = libmastodont.a # shared
AR = ar

all: static

static: $(OBJ)
	$(AR) rcs $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)
