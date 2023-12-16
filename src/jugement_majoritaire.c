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
    int votesMention[6];
    int mentionMajoritaire;
} Candidat;

int max(int a, int b) {
    if (a>=b){
        return a;
    }
    return b;
}

bool csvIsCondorcet(DataFrame *df) {
    for (int i = 4; i < df->num_columns; i++) {
        for (int j = 0; j < df->num_rows; j++) {
            int note = ((int *)df->columns[i].data)[j];
            if (note > 6) {
                return true;
            }
        }
    }
    return false;
}

int convertirReponseEnNote(int reponse){
    if (reponse == 1) return 1; //TB
    if (reponse >= 2 && reponse <= 3) return 2; //B
    if (reponse >= 4 && reponse <= 5) return 3; //AB
    if (reponse >= 6 && reponse <= 7) return 4; //P
    if (reponse >= 8 && reponse <= 9) return 5; //M
    if (reponse == 10) return 6;  //A fuir
    return -1; //L'électeur ne se prononce pas;
}


void MentionMajoritaireCandidats(Candidat *candidats, int nombreCandidats, DataFrame *df,bool isCondorcet) {
    for (int i = 0; i < nombreCandidats; i++) {
        int compteurTB = 0, compteurB = 0, compteurAB = 0, compteurP = 0, compteurM = 0, compteurAFuir = 0;
        for (int j = 0; j < df->num_rows; j++) {
            int note;
            if (isCondorcet){
                note = convertirReponseEnNote(((int *)df->columns[i + 4].data)[j]);
            }
            else{
                note = ((int *)df->columns[i + 4].data)[j];
            }
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
        candidats[i].votesMention[0] = compteurTB;
        candidats[i].votesMention[1] = compteurB;
        candidats[i].votesMention[2] = compteurAB;
        candidats[i].votesMention[3] = compteurP;
        candidats[i].votesMention[4] = compteurM;
        candidats[i].votesMention[5] = compteurAFuir;
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
    bool isCondorcet = csvIsCondorcet(df);
    VoteResult result;
    int nombreCandidats = df->num_columns - 4;
    Candidat *candidats = (Candidat *)malloc(nombreCandidats * sizeof(Candidat));
    char *columns_names[df->num_columns];
    getColumnsNames(df, columns_names);
    for (int i = 0; i < nombreCandidats; i++) {
        candidats[i].nom = columns_names[i + 4];
    }
    MentionMajoritaireCandidats(candidats, nombreCandidats, df, isCondorcet);
    int gagnantIndex = 0;
    int mentionMax = candidats[gagnantIndex].mentionMajoritaire;
    for (int i = 1; i < nombreCandidats; i++) {
        if (candidats[i].mentionMajoritaire < mentionMax) {
            gagnantIndex = i;
            mentionMax = candidats[gagnantIndex].mentionMajoritaire;
        }
        else if (candidats[i].mentionMajoritaire == mentionMax) {
            int partisansGagnant1 = 0;
            int opposantsGagnant1 = 0;
            int partisansGagnant2 = 0;
            int opposantsGagnant2 = 0;
            for (int j = 0; j < 6; j++) {
                if (j < mentionMax) {
                    partisansGagnant1 += candidats[gagnantIndex].votesMention[j];
                    partisansGagnant2 += candidats[i].votesMention[j];
                }
                else if (j>mentionMax) {
                    opposantsGagnant1 += candidats[gagnantIndex].votesMention[j];
                    opposantsGagnant2 += candidats[i].votesMention[j];
                }
            }
            int poidsMax = max(max(partisansGagnant1,partisansGagnant2),max(opposantsGagnant1,opposantsGagnant2));
            if (poidsMax==partisansGagnant2 || poidsMax == opposantsGagnant1){
                gagnantIndex = i;
                mentionMax = candidats[gagnantIndex].mentionMajoritaire;
            };
        }
    }

    // Remplir la structure de résultats
    result.nb_candidates = nombreCandidats;
    result.nb_voters = df->num_rows;
    strcpy(result.winner, candidats[gagnantIndex].nom);
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
