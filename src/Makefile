scrutin : main.o lecture_csv.o uninominales.o
	gcc main.o lecture_csv.o uninominales.o -o scrutin

lecture_csv.o : lecture_csv.c
	gcc -c lecture_csv.c -o lecture_csv.o

utils.o : utils.c
	gcc -c utils.c -o utils.o

main.o : main.c
	gcc -c main.c -o main.o

uninominales.o : uninominales.c
	gcc -c uninominales.c -o uninominales.o

pure :
	rm *.o

clean :
	rm scrutin *.o: