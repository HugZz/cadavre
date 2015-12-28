CC=gcc
CFLAGS=-Wall -g
LDFLAGS=
EXEC=cadavre
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC) clean

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm $(OBJ)
	clear
