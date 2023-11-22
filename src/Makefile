scrutin : main.o utils.o uninominales.o lecture_csv.o
	gcc main.o utils.o uninominales.o lecture_csv.o -o scrutin

main.o : main.c
	gcc -c main.c -o main.o

uninominales.o : uninominales.c
	gcc -c uninominales.c -o uninominales.o

utils.o : utils.c
	gcc -c utils.c -o utils.o

lecture_csv.o : lecture_csv.c
	gcc -c lecture_csv.c -o lecture_csv.o

clean :
	rm -rf scrutin *.o