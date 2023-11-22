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

#define MAXCHAR 1024

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

////////////////////////////////////////////////
// -- Structures de données pour DataFrame -- //
////////////////////////////////////////////////

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
 * @struct Column
 * @brief Structure de données d'une colonne
 * @details Cette structure permet de stocker les informations d'une colonne d'un DataFrame.
 *         Elle contient le nom de la colonne, son type de données et un tableau de données.
 */
typedef struct
{
    /// @brief Nom de la colonne
    char *name;
    /// @brief Type de données de la colonne
    enum DataType ctype;
    /// @brief Tableau de données de la colonne, de type void pour pouvoir stocker des données de différents types
    void *data;
} Column;

/**
 * @struct DataFrame
 * @brief Structure de données d'un DataFrame
 * @details Cette structure permet de stocker les informations d'un DataFrame.
 *         Elle contient un tableau de colonnes, le nombre de colonnes et le nombre de lignes.
 *         L'avatar de cette structure est un tableau à deux dimensions, où chaque ligne représente une colonne et chaque colonne représente une ligne.
 *         On peut donc accéder facilement à une colonne ou à une ligne en fonction de l'index de la colonne ou de la ligne.
 */
typedef struct
{
    char *index_name; ///< Nom de l'index du DataFrame.
    int num_columns;  ///< Nombre de colonnes dans le DataFrame.
    int num_rows;     ///< Nombre de lignes dans le DataFrame.
    char delimiter;   ///< Délimiteur de colonnes dans le fichier CSV.
    Column *columns;  ///< Tableau de colonnes du DataFrame.
} DataFrame;

typedef struct
{
    char *label;
    enum DataType type;
    union
    {
        int *int_value;
        double *double_value;
        time_t *timestamp_value;
        char **string_value;
    } value;
} Item;

typedef struct
{
    int nb_items;
    Item *items;
} Series;

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
int isIn(DataFrame *df, char *column_name, char *value);

/**
 * @fn void getColumnsNames(DataFrame *df, char *columns_names[df->num_columns])
 * @brief Fonction de récupération des noms des colonnes d'un DataFrame.
 * @param[in] df Pointeur vers le DataFrame dont on veut récupérer les noms de colonnes.
 * @param[out] columns_names Tableau de chaînes de caractères dans lequel stocker les noms de colonnes.
 *
 * Cette fonction récupère les noms des colonnes d'un DataFrame et les stocke dans un tableau de chaînes de caractères.
 */
void getColumnsNames(DataFrame *df, char *columns_names[]);

/**
 * @fn Column dfSelect(DataFrame *df, char *column_name)
 * @brief Fonction de sélection d'une colonne dans un DataFrame.
 * @param[in] df Pointeur vers le DataFrame dans lequel effectuer la sélection.
 * @param[in] column_name Nom de la colonne à sélectionner.
 * @return Colonne sélectionnée.
 *
 * Cette fonction sélectionne une colonne dans un DataFrame par son nom et la renvoie.
 */
Column dfSelect(DataFrame *df, char *column_name);

/**
 * @fn DataFrame getRow(DataFrame *df, char *column_name, char *value)
 * @brief Fonction de récupération d'une ligne dans un DataFrame.
 * @param[in] df Pointeur vers le DataFrame dans lequel effectuer la recherche.
 * @param[in] column_name Nom de la colonne dans laquelle rechercher.
 * @param[in] value Valeur servant à identifier la ligne à récupérer.
 * @return DataFrame contenant la ligne trouvée.
 *
 * Cette fonction recherche une valeur donnée dans une colonne d'un DataFrame et renvoie la ligne correspondante sous la forme d'une Series.
 */
Series getRow(DataFrame *df, char *column_name, char *value);

/**
 * @fn void printSeries(Series series)
 * @brief Fonction d'affichage d'une Series dans la console.
 * @param[in] series Series à afficher.
 */
void printSeries(Series series);

/**
 * @fn vint selectIntFromSeries(Series series, char *label)
 * @brief Fonction de sélection d'un item de type entier dans une Series.
 * @param[in] series Series dans laquelle effectuer la sélection.
 * @param[in] label Label de la valeur à sélectionner.
 * @return Valeur sélectionnée.
 */
int selectIntFromSeries(Series series, char *label);

/**
 * @fn double selectDoubleFromSeries(Series series, char *label)
 * @brief Fonction de sélection d'un item de type double dans une Series.
 * @param[in] series Series dans laquelle effectuer la sélection.
 * @param[in] label Label de la valeur à sélectionner.
 * @return Valeur sélectionnée.
 */
double selectDoubleFromSeries(Series series, char *label);

/**
 * @fn time_t selectTimestampFromSeries(Series series, char *label)
 * @brief Fonction de sélection d'un item de type timestamp dans une Series.
 * @param[in] series Series dans laquelle effectuer la sélection.
 * @param[in] label Label de la valeur à sélectionner.
 * @return Valeur sélectionnée.
 */
time_t selectTimestampFromSeries(Series series, char *label);

/**
 * @fn char *selectStringFromSeries(Series series, char *label)
 * @brief Fonction de sélection d'un item de type chaîne de caractères dans une Series.
 * @param[in] series Series dans laquelle effectuer la sélection.
 * @param[in] label Label de la valeur à sélectionner.
 * @return Valeur sélectionnée.
 */
char *selectStringFromSeries(Series series, char *label);

#endif // LECTURE_CSV_H
