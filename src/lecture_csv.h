/**
 * @file lecture_csv.h
 * @brief Gestion et création des structures de données DataFrame à partir de fichiers CSV (en-tête).
 * @author Essengue Matis
 * @date 23 octobre 2023
 *
 * Fichier d'en-tête pour la gestion et la création des structures de données DataFrame à partir de fichiers CSV.
 *
 */

#ifndef LECTURE_CSV_H
#define LECTURE_CSV_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

////////////////////////////////////////////
// -- Fonctions de gestion de fichiers -- //
////////////////////////////////////////////

/**
 * @fn static FILE *openFile(char *path)
 * @brief Fonction d'ouverture d'un fichier en mode lecture seule.
 * @param[in] path Chemin du fichier à ouvrir.
 * @return Pointeur vers le fichier ouvert.
 *
 * Cette fonction ouvre un fichier en mode lecture seule. En cas d'échec, elle affiche un message d'erreur et termine le programme.
 */
static FILE *openFile(char *path);

//////////////////////////////////////////////////
// -- Structures de données pour DataFrame -- //
//////////////////////////////////////////////////

/**
 * @struct Column
 * @brief Structure de données représentant une colonne dans un DataFrame.
 */
typedef struct
{
    char *name;          ///< Nom de la colonne.
    enum DataType ctype; ///< Type de données de la colonne.
    void *data;          ///< Données de la colonne.
} Column;

/**
 * @enum DataType
 * @brief Énumération représentant les types de données des colonnes d'un DataFrame.
 */
enum DataType
{
    INT,       ///< Entier.
    DOUBLE,    ///< Nombre à virgule flottante.
    TIMESTAMP, ///< Horodatage (timestamp).
    STRING     ///< Chaîne de caractères.
};

/**
 * @struct DataFrame
 * @brief Structure de données représentant un DataFrame.
 */
typedef struct
{
    char *index_name; ///< Nom de l'index du DataFrame.
    int num_columns;  ///< Nombre de colonnes dans le DataFrame.
    int num_rows;     ///< Nombre de lignes dans le DataFrame.
    char delimiter;   ///< Délimiteur de colonnes dans le fichier CSV.
    Column *columns;  ///< Tableau de colonnes du DataFrame.
} DataFrame;

////////////////////////////////////////////
// -- Fonctions de gestion de DataFrame -- //
////////////////////////////////////////////

/**
 * @fn DataFrame *createDataFrameFromCsv(char *path)
 * @brief Fonction de création d'un DataFrame à partir d'un fichier CSV.
 * @param[in] path Chemin du fichier CSV.
 * @return Pointeur vers le DataFrame créé.
 *
 * Cette fonction crée un DataFrame à partir d'un fichier CSV situé au chemin spécifié. Elle alloue la mémoire nécessaire, lit les données depuis le fichier CSV et remplit le DataFrame. En cas d'erreur, elle renvoie NULL.
 */
DataFrame *createDataFrameFromCsv(char *path);

/**
 * @fn void printDf(DataFrame *df)
 * @brief Fonction d'affichage d'un DataFrame dans la console.
 * @param[in] df Pointeur vers le DataFrame à afficher.
 *
 * Cette fonction affiche les informations sur un DataFrame, y compris le nombre de colonnes et de lignes, ainsi que les noms des colonnes. Elle affiche ensuite le contenu de chaque colonne, aligné selon le type de données.
 */
void printDf(DataFrame *df);

/**
 * @fn int findColumn(DataFrame *df, char *column_name)
 * @brief Fonction de recherche d'une colonne dans un DataFrame par nom.
 * @param[in] df Pointeur vers le DataFrame dans lequel effectuer la recherche.
 * @param[in] column_name Nom de la colonne à rechercher.
 * @return Indice de la colonne si trouvée, -1 sinon.
 *
 * Cette fonction recherche une colonne dans un DataFrame par son nom. Si la colonne est trouvée, elle renvoie l'indice de la colonne. Sinon, elle renvoie -1.
 */
int findColumn(DataFrame *df, char *column_name);

/**
 * @fn bool isIn(DataFrame *df, char *column_name, char *value)
 * @brief Fonction de recherche d'une valeur dans une colonne d'un DataFrame.
 * @param[in] df Pointeur vers le DataFrame dans lequel effectuer la recherche.
 * @param[in] column_name Nom de la colonne dans laquelle rechercher.
 * @param[in] value Valeur à rechercher (int, double ou string).
 * @return true si la valeur est trouvée, false sinon.
 *
 * Cette fonction recherche une valeur donnée dans une colonne d'un DataFrame. Si la colonne ou la valeur n'est pas trouvée, elle renvoie false. Sinon, elle renvoie true.
 */
bool isIn(DataFrame *df, char *column_name, char *value);

#endif // LECTURE_CSV_H
