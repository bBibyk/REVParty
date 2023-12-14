CC = gcc
FLAGS = -Werror -Wall -pedantic

scrutin : main.o utils.o uninominales.o lecture_csv.o jugement_majoritaire.o condrcet.o graph.o
	$(CC) $(FLAGS) main.o utils.o uninominales.o jugement_majoritaire.o condorcet.o graph.o lecture_csv.o -o scrutin

main.o : main.c
	$(CC) $(FLAGS) -c main.c -o main.o

uninominales.o : uninominales.c
	$(CC) $(FLAGS) -c uninominales.c -o uninominales.o

jugement_majoritaire.o : jugement_majoritaire.c
	$(CC) $(FLAGS) -c jugement_majoritaire.c -o jugement_majoritaire.o

condrcet.o : condorcet.c
	$(CC) $(FLAGS) -c condorcet.c -o condorcet.o

graph.o : graph.c
	$(CC) $(FLAGS) -c graph.c -o graph.o

utils.o : utils.c
	$(CC) $(FLAGS) -c utils.c -o utils.o

lecture_csv.o : lecture_csv.c
	$(CC) $(FLAGS) -c lecture_csv.c -o lecture_csv.o

clean :
	rm -rf scrutin *.o