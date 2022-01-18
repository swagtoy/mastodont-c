CC ?= cc
CFLAGS = -I ./include/
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
