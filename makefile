CC=gcc

SRC = driver.c parser.c lexer.c

OBJ = $(SRC:.c=.o)

EXEC=a.out

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $<

# Clean build files
clean:
	rm -f $(OBJ) $(EXEC)
