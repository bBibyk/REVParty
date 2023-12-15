/**
 * @file jugement_majoritaire.c
 * @brief Implémentation de la méthode de vote par jugement majoritaire, avec stockage des résultats dans une structure VoteResult.
 * @author Pigani Emanuele
 * @date  01 décembre 2023
 *
 * Ce fichier contient l'implémentation de la méthode de jugement majoritaire ainsi que les fonctions auxiliaires au développement de la methode.
 * Les résultats du vote sont stockés dans une structure VoteResult.
 */

#ifndef JUGEMENT_MAJORITAIRE_C
#define JUGEMENT_MAJORITAIRE_C

#include "jugement_majoritaire.h"

// Structure pour stocker les Candidats et leur mention majoritaire (Pour départage)
typedef struct {
    char *nom;
    int mentionMajoritaire;
} Candidat;


int convertirReponseEnNote(int reponse){
    if (reponse == 1) return 1; //TB
    if (reponse >= 2 && reponse <= 3) return 2; //B
    if (reponse >= 4 && reponse <= 5) return 3; //AB
    if (reponse >= 6 && reponse <= 7) return 4; //P
    if (reponse >= 8 && reponse <= 9) return 5; //M
    if (reponse == 10) return 6;  //A fuir
    return -1; //L'électeur ne se prononce pas;
}


void MentionMajoritaireCandidats(Candidat *candidats, int nombreCandidats, DataFrame *df) {
    for (int i = 0; i < nombreCandidats; i++) {
        int compteurTB = 0, compteurB = 0, compteurAB = 0, compteurP = 0, compteurM = 0, compteurAFuir = 0;
        for (int j = 0; j < df->num_rows; j++) {
            int note = convertirReponseEnNote(((int *)df->columns[i + 4].data)[j]);
            if (note == 1) {
                compteurTB++;
            } 
            else if (note == 2) {
                compteurB++;
            } 
            else if (note == 3) {
                compteurAB++;
            }
            else if (note == 4) {
                compteurP++;
            } 
            else if (note == 5) {
                compteurM++;
            } 
            else if (note == 6) {
                compteurAFuir++;
            } 
            else {
                continue;
            }
        }
        // Recherche par médiane du jugement majoritaire du candidat
        if (compteurTB > (df->num_rows / 2)) {
            candidats[i].mentionMajoritaire = 1;
        } else if (compteurB + compteurTB > (df->num_rows / 2)) {
            candidats[i].mentionMajoritaire = 2;
        } else if (compteurAB + compteurB + compteurTB > (df->num_rows / 2)) {
            candidats[i].mentionMajoritaire = 3;
        } else if (compteurP + compteurAB + compteurB + compteurTB > (df->num_rows / 2)) {
            candidats[i].mentionMajoritaire = 4;
        } else if (compteurM + compteurP + compteurAB + compteurB + compteurTB > (df->num_rows / 2)) {
            candidats[i].mentionMajoritaire = 5;
        } else {
            candidats[i].mentionMajoritaire = 6;
        }
    }
}



VoteResult voteJugementMajoritaire(DataFrame *df, FILE *log, bool debugMode){
    VoteResult result;
    int nombreCandidats = df->num_columns - 4;
    Candidat *candidats = (Candidat *)malloc(nombreCandidats * sizeof(Candidat));
    char *columns_names[df->num_columns];
    getColumnsNames(df, columns_names);
    for (int i = 0; i < nombreCandidats; i++) {
        candidats[i].nom = columns_names[i + 4];
    }
    MentionMajoritaireCandidats(candidats, nombreCandidats, df);
    int gagnantIndex = 0;
    for (int i = 1; i < nombreCandidats; i++) {
        if (candidats[i].mentionMajoritaire < candidats[gagnantIndex].mentionMajoritaire) {
            gagnantIndex = i;
        }
    }
    result.nb_candidates = nombreCandidats;
    result.nb_voters = df->num_rows;
    strcpy(result.winner, candidats[gagnantIndex].nom);
    // Ecriture des résultats du vote dans le fichier de log
    if (debugMode) {
        fprintf(log, "Résultats du vote par jugement majoritaire :\n");
        fprintf(log, "Gagnant : %s\n", result.winner);
        fprintf(log, "Nombre de candidats : %d\n", result.nb_candidates);
        fprintf(log, "Nombre d'électeurs : %d\n", result.nb_voters);
        fprintf(log, "\n");
    }
    return result;
}


#endif
