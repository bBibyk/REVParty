/**
 * @file verify_my_vote.c
 * @brief Vérification de la présence et du résultat d'un vote dans un fichier CSV pour un nom et un code donné.
 * @author Essengue Matis
 * @date 27 octobre 2023
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lecture_csv.h"
#include "../Sha256/sha256_utils.h"

void toUpperCase(char *str)
{
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}

void toCamelCase(char *str)
{
    str[0] = toupper((unsigned char)str[0]);
    for (int i = 1; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

void getHash(DataFrame *df_codes,
             char *num_etu,
             char *nom,
             char *prenom,
             char hash_res[SHA256_BLOCK_SIZE * 2 + 1])
{
    // -- On commence par récuperer le code personnel
    Series infosEtu = getRow(df_codes, "Electeur", num_etu);
    char *code_perso = selectStringFromSeries(infosEtu, "Clef");

    // -- Puis on crée le nom complet qu'on hash ensuite
    char nom_complet[128];
    toUpperCase(nom);
    toCamelCase(prenom);
    snprintf(nom_complet, sizeof(nom_complet), "%s %s%s", nom, prenom, code_perso);
    sha256ofString(nom_complet, hash_res);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s [NOM] [Prénom] [numéro d'étudiant]\n", argv[0]);
        return 1;
    }

    if (atoi(argv[3]) == 0)
    {
        fprintf(stderr, "Error: [numéro d'étudiant] doit être un nombre.\n");
        return 2;
    }

    char *nom = argv[1];
    char *prenom = argv[2];
    char *num_etu = argv[3];

    DataFrame *df_codes = createDataFrameFromCsv("../data/codeCondorcetNumEtu.csv");
    DataFrame *df_res_votes = createDataFrameFromCsv("../data/VoteCondorcet.csv");
    char hash_res[SHA256_BLOCK_SIZE * 2 + 1];
    getHash(df_codes, num_etu, nom, prenom, hash_res);

    if (isIn(df_res_votes, "Nom complet", hash_res) == -1)
        printf("Vous n'avez pas encore voter!");

    else
    {
        Series vote_details = getRow(df_res_votes, "Nom complet", hash_res);
        printf("Voici les détails de votre vote :\n\n");
        printSeries(vote_details);
    }

    return 0;
}