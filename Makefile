OPT = -Wall
OBJ = main.o room.o ghost.o hunter.o evidence.o building.o

proj: $(OBJ)
	gcc $(OPT) -o proj $(OBJ) -lpthread

main.o: main.c defs.h
	gcc $(OPT) -c main.c

room.o: room.c defs.h
	gcc $(OPT) -c room.c

ghost.o: ghost.c defs.h
	gcc $(OPT) -c ghost.c

hunter.o: hunter.c defs.h
	gcc $(OPT) -c hunter.c

evidence.o: evidence.c defs.h
	gcc $(OPT) -c evidence.c

building.o: building.c defs.h
	gcc $(OPT) -c building.c

clean:
	rm -f $(OBJ) proj
