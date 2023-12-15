# Variables de compilation à modifier si besoin
CC = gcc
FLAGS = -Werror -Wall -pedantic
TARGET = bin/scrutin

# Creation de liste de fichiers à traiter
OBJS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c)) # fichiers objets

# Règle principale du make et edition des liens 
$(TARGET) : $(OBJS) | bin
	@echo "Edition des liens..."
	@$(CC) $(CFLAGS) $^ -o $@

# Règle cachée de compilation
obj/%.o : src/%.c | obj
	@echo "Compilation de $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Règles cachées de création des répertioires manquants
obj :
	@echo "Création du répertoire $@..."
	@mkdir -p $@

bin :
	@echo "Création du répertoire $@..."
	@mkdir -p $@

# Règle secondaire de création de l'éxecutable pour la vérification de vote
# Cette règle n'est pas généralisée, parce qu'elle reste détachée du programme.
verify_my_vote : src/verify_my_vote/verify_my_vote.c Sha256/sha256_utils.c Sha256/sha256_utils.c | obj/lecture_csv.o
	@$(CC) $(CFLAGS) -c Sha256/sha256.c -o obj/sha256.o
	@$(CC) $(CFLAGS) -c Sha256/sha256_utils.c -o obj/sha256_utils.o
	@$(CC) $(CFLAGS) -c src/$@/$@.c -o obj/$@.o
	@$(CC) $(CFLAGS) obj/$@.o obj/lecture_csv.o obj/sha256_utils.o obj/sha256.o -o bin/$@

# Règle secondaire de production de la documentation
docs : 
	@echo "Production de la documentation..."
	@doxygen docs/Doxyfile

# Règle secondaire de nettoyage
clean :
	@echo "Supression de tous les fichiers de compilation..."
	@rm -rf bin/scrutin bin/verify_my_vote obj/*.o