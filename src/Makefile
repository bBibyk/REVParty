scrutin : main.o utils.o uninominales.o lecture_csv.o jugement_majoritaire.o condrcet.o graph.o
	gcc main.o utils.o uninominales.o jugement_majoritaire.o condorcet.o graph.o lecture_csv.o -o scrutin

main.o : main.c
	gcc -c main.c -o main.o

uninominales.o : uninominales.c
	gcc -c uninominales.c -o uninominales.o

jugement_majoritaire.o : jugement_majoritaire.c
	gcc -c jugement_majoritaire.c -o jugement_majoritaire.o

condrcet.o : condorcet.c
	gcc -c condorcet.c -o condorcet.o

graph.o : graph.c
	gcc -c graph.c -o graph.o

utils.o : utils.c
	gcc -c utils.c -o utils.o

lecture_csv.o : lecture_csv.c
	gcc -c lecture_csv.c -o lecture_csv.o

clean :
	rm -rf scrutin *.o