/**
 * @file main.c
 * @brief Analyse de la ligne de commande et appel de la fonction appropriée, gestion des sorties
 * @author Bibyk Bogdan
 * @date 31 octobre 2023
 *
 * Fonctions de gestion de la ligne de commande et du noyau fonctionnel.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "lecture_csv.h"
#include "condorcet.h"
#include "jugement_majoritaire.h"
#include "uninominales.h"
//TODO : includes

////////////////////////////////////////////////////////
// -- Fonctions de gestion de la ligne de commande -- //
////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    int f;
    f = getopt(argc, argv, "i:d:om:");
    printf("%c - %s\n", f, optarg);
    f = getopt(argc, argv, "i:d:om:");
    printf("%c - %s\n", f, optarg);
}