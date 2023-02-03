EXEC1 = t1
SRC1 = t1.c 
OBJ1 = $(SRC1:.c=.o)

EXEC2 = main
SRC2 = main.c fonction.c
OBJ2 = $(SRC2:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJ1)
	$(CC) -o $(EXEC1) $(OBJ1)

$(EXEC2): $(OBJ2)
	$(CC) -o $(EXEC2) $(OBJ2)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ1) $(OBJ2)
	rm -f $(EXEC1) $(EXEC2)

.PHONY: all clean
