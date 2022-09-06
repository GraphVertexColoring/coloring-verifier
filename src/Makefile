PROG = ./coloring-verifier
#CFLAGS = -Wall -static -ansi -pedantic
#LDFLAGS = -lm  

OBJ=bin2asc.o test_sol.o

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) main.c -o $(PROG) $(LDFLAGS)

clean:
	rm -f *~ *.o $(PROG)


test:
	${PROG} -i ../examples/marco10.col -s ../examples/marco10.sol -p 1
