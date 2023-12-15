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
#include "../lecture_csv.h"
#include "../../Sha256/sha256_utils.h"

/**
 * @fn void toUpperCase(char *str)
 * @brief Fonction de mise en majuscule d'une chaîne de caractères.
 * @param[in,out] str Chaîne de caractères à mettre en majuscule.
 *
 * @details Une fonction qui prend en parametre une chaine de charactere dans un format quelconque (majuscule ou non), et qui la met en majuscule.
 */
void toUpperCase(char *str)
{
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}

/**
 * @fn void toCamelCase(char *str)
 * @brief Fonction de mise en camelCase d'une chaîne de caractères.
 * @param[in,out] str Chaîne de caractères à mettre en camelCase.
 *
 * @details Une fonction qui prend en parametre une chaine de charactere dans un format quelconque et qui met la première lettre en majuscule et le reste en minuscule.
 */
void toCamelCase(char *str)
{
    str[0] = toupper((unsigned char)str[0]);
    for (int i = 1; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

/**
 * @fn void getHash(DataFrame *df_codes, char *num_etu, char *nom, char *prenom, char hash_res[SHA256_BLOCK_SIZE * 2 + 1])
 * @brief Fonction de récupération du hash d'un vote à partir d'un nom, d'un prénom et d'un numéro d'étudiant.
 * @param[in] df_codes DataFrame contenant les codes personnels des électeurs et leur numéro d'étudiant.
 * @param[in] num_etu Numéro d'étudiant de l'électeur.
 * @param[in] nom Nom de l'électeur.
 * @param[in] prenom Prénom de l'électeur.
 * @param[out] hash_res Résultat du hash.
 *
 * @details Une fonction qui récupère le code personnel de l'électeur à partir de son numéro d'étudiant, puis qui crée le nom complet de l'électeur et le hash.
 */
void getHash(DataFrame *df_codes,
             char *num_etu,
             char *nom,
             char *prenom,
             char hash_res[SHA256_BLOCK_SIZE * 2 + 1])
{
    // -- On commence par récuperer le code personnel
    Series infosEtu = getRow(df_codes, "Electeur", num_etu);
    char *code_perso = selectStringFromSeries(infosEtu, "Clef");

    // -- Puis on crée le nom complet et on hash
    char nom_complet[128];
    toUpperCase(nom);
    toCamelCase(prenom);
    snprintf(nom_complet, sizeof(nom_complet), "%s %s%s", nom, prenom, code_perso);
    sha256ofString(nom_complet, hash_res);
}

/**
 * @fn int main(int argc, char *argv[])
 * @brief Fonction principale.
 * @param[in] argc Nombre d'arguments.
 * @param[in] argv Arguments.
 * @return 0 si tout s'est bien passé, 1 si le nombre d'arguments est incorrect, 2 si le numéro d'étudiant n'est pas un nombre.
 *
 * @details Une fonction qui récupère le nom, le prénom et le numéro d'étudiant, puis elle récupère le hash du vote correspondant. Enfin, elle vérifie si le hash est présent dans le fichier des résultats des votes.
 *          Si c'est le cas, elle affiche les détails du vote, sinon elle affiche un message d'erreur.
 */
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
