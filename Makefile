# Variables à modifier si besoin
CC = gcc
FLAGS = -Werror -Wall -pedantic
TARGET = bin/scrutin

# Creation des listes de fichiers à traiter
SRCS = $(wildcard src/*.c) # fichiers sources
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS)) # fichiers objets

# Commande principale du make et edition des liens 
$(TARGET) : $(OBJS)
	@echo "Edition des liens..."
	@$(CC) $(CFLAGS) $^ -o $@

# Commande secondaire de compilation
obj/%.o : src/%.c | obj
	@echo "compilation de $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Commande secondaire de production de la documentation
docs : 
	doxygen docs/Doxyfile

# Commande secondaire de nettoyage
clean :
	@echo "Supression de tous les fichiers de compilation..."
	@rm -rf bin/scrutin obj/*.o