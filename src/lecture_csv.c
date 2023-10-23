/**
 * @file lecture_csv.c
 * @brief Gestion et création des structures de données DataFrame à partir de fichiers CSV.
 * @author Essengue Matis
 * @date 22 octobre 2023
 *
 * Fonctions de gestion et de création des structures de données DataFrame à partir de fichiers CSV.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

/// @def MAXCHAR
#define MAXCHAR 1024

////////////////////////////////////////////
// -- Fonctions de gestion de fichiers -- //
////////////////////////////////////////////

/**
 * @fn static FILE *openFile(char *path)
 * @brief Fonction d'ouverture d'un fichier en mode lecture seule
 * @param[in] path Chemin du fichier à ouvrir
 * @return Pointeur vers le fichier ouvert
 *
 * @note Cette fonction affiche un message d'erreur si le fichier n'a pas pu être ouvert.
 */
static FILE *openFile(char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    return fp;
}

/**
 * @fn static void closeFile(FILE *fp)
 * @brief Fonction de fermeture d'un fichier
 * @param[in] fp Pointeur vers le fichier à fermer
 *
 * @note Cette fonction affiche un message d'erreur si le fichier n'a pas pu être fermé.
 */
static void closeFile(FILE *fp)
{
    if (fclose(fp) != 0)
    {
        perror("fclose");
        exit(EXIT_FAILURE);
    }
}

////////////////////////////////////////////////////
// -- Fonctions de lecture/analyse de fichiers -- //
////////////////////////////////////////////////////

/**
 * @fn static char getDelimiter(FILE *fp)
 * @brief Fonction d'identification du délimiteur d'un fichier CSV
 * @param[in] fp Pointeur vers le fichier à analyser
 * @return Délimiteur du fichier
 *
 * @details Cette fonction permet d'identifier le délimiteur d'un fichier CSV parmis ','|' '|';'.
 * @note Cette fonction affiche un message d'erreur si le fichier ne contient pas de délimiteur.
 */
static char getDelimiter(FILE *fp)
{
    // On commence par retourner au début du fichier
    fseek(fp, 0, SEEK_SET);

    // On lit la première ligne du fichier
    char row[MAXCHAR];
    fgets(row, MAXCHAR, fp);

    // On parcourt la ligne pour trouver le délimiteur
    int i = 0;
    while (row[i] != '\0')
    {
        if (row[i] == ';')
            return ';';
        else if (row[i] == ',')
            return ',';
        else if (row[i] == ' ')
            return ' ';
        i++;
    }

    // Si on n'a pas trouvé de délimiteur, on affiche un message d'erreur et on quitte le programme
    fprintf(stderr, "Erreur : le fichier ne contient pas de délimiteur.\n");
    exit(EXIT_FAILURE);
}

/**
 * @fn static int getNumColumns(FILE *fp, char delimiter)
 * @brief Fonction de comptage du nombre de colonnes d'un fichier CSV
 * @param[in] fp Pointeur vers le fichier à analyser
 * @param[in] delimiter Délimiteur du fichier
 * @return Nombre de colonnes du fichier
 */
static int getNumColumns(FILE *fp, char delimiter)
{
    // On commence par retourner au début du fichier
    int columns_number = 0;
    fseek(fp, 0, SEEK_SET);

    // On lit la première ligne du fichier
    char row[MAXCHAR];
    fgets(row, MAXCHAR, fp);

    // On parcourt la ligne pour compter le nombre de délimiteurs
    int i = 0;
    while (row[i] != '\0')
    {
        if (row[i] == delimiter)
            columns_number++;
        i++;
    }

    // On retourne le nombre de délimiteurs + 1, soit le nombre de colonnes
    return columns_number + 1;
}

/**
 * @fn static int getNumRows(FILE *fp)
 * @brief Fonction de comptage du nombre de lignes d'un fichier CSV
 * @param[in] fp Pointeur vers le fichier à analyser
 * @return Nombre de lignes du fichier
 */
static int getNumRows(FILE *fp)
{
    // On commence par retourner au début du fichier
    fseek(fp, 0, SEEK_SET);

    // On compte le nombre de lignes du fichier
    int num_rows = 0;
    char row[MAXCHAR];
    while (fgets(row, MAXCHAR, fp) != NULL)
        num_rows++;

    // On retourne le nombre de lignes - 1, soit le nombre de lignes sans le nom des colonnes
    return num_rows - 1;
}

/**
 * @fn static void getRowElem(char row[MAXCHAR], char delimiter, char *data[])
 * @brief Fonction de récupération des éléments d'une ligne d'un fichier CSV
 * @param[in] row Ligne du fichier à analyser
 * @param[in] delimiter Délimiteur du fichier
 * @param[out] data Tableau contenant les éléments de la ligne
 *
 * @details Cette fonction découpe une ligne de fichier CSV en fonction du délimiteur et stocke les éléments dans un tableau.
 */
static void getRowElem(char row[MAXCHAR], char delimiter, char *data[])
{
    // La fonction strtok() découpe une chaîne de caractères en fonction d'un délimiteur
    char *token = strtok(row, &delimiter);
    int i = 0;
    while (token != NULL)
    {
        data[i] = token;
        token = strtok(NULL, &delimiter); // On passe NULL pour continuer à découper la chaîne précédente
        i++;
    }
}

/**
 * @enum DataType
 * @brief Enumération des types de données
 * @details Cette énumération permet de définir les types de données possibles pour une colonne (INT, DOUBLE, TIMESTAMP, STRING).
 */
enum DataType
{
    INT,
    DOUBLE,
    TIMESTAMP,
    STRING
};

/**
 * @fn static enum DataType getColumnType(char *data)
 * @brief Fonction d'identification du type de données d'une colonne
 * @param[in] data Tableau contenant les éléments d'une colonne
 * @return Type de données de la colonne
 *
 * @details Cette fonction permet d'identifier le type de données d'une colonne parmis INT, DOUBLE, TIMESTAMP, STRING, en fonction de son premier élément.
 */
static enum DataType
getColumnType(char *data)
{
    char *endptr;
    long int_value = strtol(data, &endptr, 10);
    if (*endptr == '\0')
        return INT;

    double double_value = strtod(data, &endptr);
    if (*endptr == '\0')
        return DOUBLE;

    if (strlen(data) == 19)
    {
        char year[5], month[3], day[3], hour[3], minute[3], second[3];
        if (sscanf(data, "%2[^/]/%2[^/]/%4[^ ] %2[^:]:%2[^:]:%2s", day, month, year, hour, minute, second) == 6)
            return TIMESTAMP;
    }

    return STRING;
}

/**
 * @fn static void getColumnsType(char *data[], int num_columns, enum DataType columns_type[num_columns])
 * @brief Fonction de récupération des types de données des colonnes d'un fichier CSV
 * @param[in] data Tableau contenant les éléments d'une ligne
 * @param[in] num_columns Nombre de colonnes du fichier
 * @param[out] columns_type Tableau contenant les types de données des colonnes
 *
 * @details Cette fonction permet de récupérer les types de données des colonnes d'un fichier CSV, à partir d'une ligne de ce fichier.
 */
static void getColumnsType(char *data[], int num_columns, enum DataType columns_type[num_columns])
{
    for (int i = 0; i < num_columns; i++)
        columns_type[i] = getColumnType(data[i]);
}

/////////////////////////////////////////////
// -- Fonctions de gestion de DataFrame -- //
/////////////////////////////////////////////

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
    /// @brief Tableau de colonnes
    Column *columns;
    /// @brief Nombre de colonnes
    int num_columns;
    /// @brief Nombre de lignes
    int num_rows;
} DataFrame;

/**
 * @fn static int getColumnIndex(DataFrame *df, char *column_name)
 * @brief Fonction de récupération de l'index d'une colonne
 * @param[in] df DataFrame
 * @param[in] column_name Nom de la colonne
 * @return Index de la colonne
 *
 * @details Cette fonction permet de récupérer l'index d'une colonne à partir de son nom.
 *         Si la colonne n'existe pas, la fonction retourne -1.
 */
static int getColumnIndex(DataFrame *df, char *column_name)
{
    for (int i = 0; i < df->num_columns; i++)
    {
        if (strcmp(df->columns[i].name, column_name) == 0) // La fonction strcmp() renvoie 0 si les deux chaînes sont identiques
            return i;
    }
    return -1;
}

/**
 * @fn void strToTimestamp(char *data, time_t *timestamp)
 * @brief Fonction de conversion d'une chaîne de caractères en timestamp
 * @param[in] data Chaîne de caractères
 * @param[out] timestamp Timestamp
 *
 * @details Cette fonction permet de convertir une chaîne de caractères au format "dd/mm/yyyy hh:mm:ss" en timestamp.
 */
static void strToTimestamp(char *data, time_t *timestamp)
{
    char year[5], month[3], day[3], hour[3], minute[3], second[3];
    sscanf(data, "%2[^/]/%2[^/]/%4[^ ] %2[^:]:%2[^:]:%2s", day, month, year, hour, minute, second);
    struct tm tm;
    tm.tm_year = atoi(year) - 1900;
    tm.tm_mon = atoi(month) - 1;
    tm.tm_mday = atoi(day);
    tm.tm_hour = atoi(hour);
    tm.tm_min = atoi(minute);
    tm.tm_sec = atoi(second);
    *timestamp = mktime(&tm);
}

/**
 * @fn void freeDataFrame(DataFrame *df)
 * @brief Fonction de libération de la mémoire allouée à un DataFrame et à ses colonnes et données associées
 * @param[in, out] df DataFrame
 */
void freeDataFrame(DataFrame *df)
{
    for (int i = 0; i < df->num_columns; i++)
    {
        free(df->columns[i].name);
        // Si le type de données de la colonne est STRING, on libère la mémoire allouée à chaque chaîne de caractères
        if (df->columns[i].ctype == STRING)
        {
            char **column_data = (char **)df->columns[i].data;
            for (int row = 0; row < df->num_rows; row++)
            {
                free(column_data[row]);
            }
        }
        free(df->columns[i].data);
    }
    free(df->columns);
    free(df);
}

/**
 * @fn static void throwAllocationError()
 * @brief Fonction de gestion d'erreur d'allocation mémoire
 * @details Cette fonction permet de gérer les erreurs d'allocation mémoire.
 *         Si l'allocation mémoire échoue, la fonction affiche un message d'erreur et termine le programme.
 */
static void throwAllocationError()
{
    perror("Erreur d'allocation mémoire");
    exit(EXIT_FAILURE);
}

/**
 * @fn static void allocateDfMem(DataFrame **df)
 * @brief Fonction d'allocation mémoire d'un DataFrame
 * @param[out] df DataFrame
 *
 * @details Cette fonction permet d'allouer la mémoire nécessaire à un DataFrame.
 *
 * @note Si la mémoire ne peut pas être allouée, la fonction renvoie une erreur.
 */
static void allocateDfMem(DataFrame **df)
{
    *df = (DataFrame *)malloc(sizeof(DataFrame));
    if (*df == NULL)
        throwAllocationError();
}

/**
 * @fn static void loadCsvMetadata(DataFrame *df, FILE *fp, char delimiter)
 * @brief Fonction de récupération des métadonnées d'un DataFrame à partir d'un fichier CSV
 * @param[out] df DataFrame
 * @param[in] fp FILE
 * @param[in] delimiter char
 *
 * @details Cette fonction permet de récupérer les métadonnées d'un DataFrame à partir d'un fichier CSV.
 *        Les métadonnées sont le nombre de colonnes et le nombre de lignes.
 */
static void loadCsvMetadata(DataFrame *df, FILE *fp, char delimiter)
{
    int num_columns = getNumColumns(fp, delimiter);
    int num_rows = getNumRows(fp);
    df->num_columns = num_columns;
    df->num_rows = num_rows;
}

/**
 * @fn static void allocateColumnsMem(DataFrame *df)
 * @brief Fonction d'allocation mémoire des colonnes d'un DataFrame à partir de ses métadonnées
 * @param[out] df DataFrame
 *
 * @note Cette fonction doit être appelée après la fonction loadCsvMetadata().
 * @note Si la fonction ne parvient pas à allouer la mémoire nécessaire, elle retourne une erreur.
 */
static void allocateColumnsMem(DataFrame *df)
{
    df->columns = (Column *)malloc(df->num_columns * sizeof(Column));
    if (df->columns == NULL)
        throwAllocationError();
}

/**
 * @fn static void nextRow(DataFrame *df, FILE *fp, char delimiter, char *row, char **data)
 * @brief Fonction de mise à jour d'une ligne d'un DataFrame
 * @param[in, out] df DataFrame
 * @param[in] fp FILE
 * @param[in] delimiter char
 * @param[out] row char
 * @param[out] data char**
 *
 * @details Cette fonction permet de mettre à jour une ligne d'un DataFrame à partir d'un fichier CSV.
 *         La ligne est stockée dans un tableau de chaînes de caractères.
 *         Les données sont ensuites séparées et stockées dans un tableau de chaînes de caractères grâce à la fonction getRowElem().
 */
static void nextRow(DataFrame *df, FILE *fp, char delimiter, char *row, char **data)
{
    fgets(row, MAXCHAR, fp);
    row[strcspn(row, "\n")] = '\0'; // Supprimer le \n
    getRowElem(row, delimiter, data);
}

/**
 * @fn static void addColumnsName(DataFrame *df, char **data)
 * @brief Fonction d'ajout des noms des colonnes d'un DataFrame à partir d'un tableau de chaînes de caractères
 * @param[in, out] df DataFrame
 * @param[in] data char**
 */
static void addColumnsName(DataFrame *df, char **data)
{
    for (int j = 0; j < df->num_columns; j++)
        df->columns[j].name = strdup(data[j]);
}

/**
 * @fn static void allocDataMem(DataFrame *df, char **data)
 * @brief Fonction d'allocation mémoire des données d'une colonne d'un DataFrame
 * @param[in, out] df DataFrame
 * @param[in] data char**
 *
 * @note Si la fonction ne parvient pas à allouer la mémoire nécessaire, elle retourne une erreur.
 */
static void allocDataMem(DataFrame *df, char **data)
{
    enum DataType columns_type[df->num_columns];
    getColumnsType(data, df->num_columns, columns_type);

    for (int j = 0; j < df->num_columns; j++)
    {
        df->columns[j].ctype = columns_type[j];
        void *column_data = NULL;

        if (columns_type[j] == INT)
            column_data = (int *)malloc(df->num_rows * sizeof(int));

        else if (columns_type[j] == DOUBLE)
            column_data = (double *)malloc(df->num_rows * sizeof(double));

        else if (columns_type[j] == TIMESTAMP)
            column_data = (time_t *)malloc(df->num_rows * sizeof(time_t));

        else if (columns_type[j] == STRING)
            column_data = (char **)malloc(df->num_rows * sizeof(char *));

        if (column_data == NULL)
            throwAllocationError();
        df->columns[j].data = column_data;
    }
}

/**
 * @fn static void fillRow(DataFrame *df, char **data, int i)
 * @brief Fonction de remplissage d'une ligne d'un DataFrame
 * @author Essengue Matis
 * @date 23 octobre 2023
 *
 * @param[in, out] df DataFrame
 * @param[in] data char**
 * @param[in] i int
 *
 * @details Cette fonction permet de remplir une ligne d'un DataFrame à partir d'un tableau de chaînes de caractères.
 */
static void fillRow(DataFrame *df, char **data, int i)
{
    int *int_data;
    double *double_data;
    time_t *timestamp_data;
    char **string_data;

    for (int j = 0; j < df->num_columns; j++)
    {
        switch (df->columns[j].ctype)
        {
        case INT:
            int_data = (int *)df->columns[j].data;
            int_data[i - 1] = atoi(data[j]);
            break;
        case DOUBLE:
            double_data = (double *)df->columns[j].data;
            double_data[i - 1] = atof(data[j]);
            break;
        case TIMESTAMP:
            timestamp_data = (time_t *)df->columns[j].data;
            strToTimestamp(data[j], &timestamp_data[i - 1]);
            break;
        case STRING:
            string_data = (char **)df->columns[j].data;
            string_data[i - 1] = strdup(data[j]);
            break;
        }
    }
}

/**
 * @fn DataFrame *createDataFrameFromCsv(char *path)
 * @brief Fonction de création d'un DataFrame à partir d'un fichier CSV
 * @author Essengue Matis
 * @date 23 octobre 2023
 *
 * @param[in] path char*
 * @return DataFrame*
 */
DataFrame *createDataFrameFromCsv(char *path)
{
    // On commence par allouer la mémoire pour le DataFrame
    DataFrame *df;
    allocateDfMem(&df);

    // Ensuite on recupere le nombre de colonnes et de lignes
    FILE *fp = openFile(path);
    char delimiter = getDelimiter(fp);
    loadCsvMetadata(df, fp, delimiter);

    // Ensuite on crée les colonnes
    allocateColumnsMem(df);

    fseek(fp, 0, SEEK_SET);
    char row[MAXCHAR];
    char *data[df->num_columns];

    // Enfin on remplit le DataFrame ligne par ligne
    for (int i = 0; i <= df->num_rows; i++)
    {
        nextRow(df, fp, delimiter, row, data);
        if (i == 0)
            addColumnsName(df, data);
        else
        {
            if (i == 1)
                allocDataMem(df, data);
            fillRow(df, data, i);
        }
    }

    closeFile(fp);
    return df;
}

/**
 * @fn void printDf(DataFrame *df)
 * @brief Fonction d'affichage d'un DataFrame dans la console
 * @author Essengue Matis
 * @date 23 octobre 2023
 *
 * @param[in] df DataFrame*
 *
 * @details Cette fonction permet d'afficher un DataFrame dans la console.
 *          Elle affiche le nom des colonnes, puis le contenu de celles-ci.
 */
void printDf(DataFrame *df)
{
    printf("Nombre de colonnes : %d, nombre de lignes : %d\n", df->num_columns, df->num_rows);

    // Afficher les noms des colonnes
    for (int i = 0; i < df->num_columns; i++)
    {
        printf("%s\t", df->columns[i].name);
    }
    printf("\n");

    int *int_data;
    double *double_data;
    time_t *timestamp_data;
    char **string_data;

    // Afficher le contenu des colonnes
    for (int row = 0; row < df->num_rows; row++)
    {
        for (int col = 0; col < df->num_columns; col++)
        {
            switch (df->columns[col].ctype)
            {
            case INT:
                int_data = (int *)df->columns[col].data;
                printf("%d\t", int_data[row]);
                break;
            case DOUBLE:
                double_data = (double *)df->columns[col].data;
                printf("%lf\t", double_data[row]);
                break;
            case TIMESTAMP:
                timestamp_data = (time_t *)df->columns[col].data;
                printf("%ld\t", timestamp_data[row]);
                break;
            case STRING:
                string_data = (char **)df->columns[col].data;
                printf("%s\t", string_data[row]);
                break;
            }
        }
        printf("\n");
    }
}

//////////////////////////////////////////////////
// -- Fonctions de manipulation de DataFrame -- //
//////////////////////////////////////////////////

/**
 * @fn int findColumn(DataFrame *df, char *column_name)
 * @brief Fonction de recherche d'une colonne dans un DataFrame à partir de son nom
 * @author Essengue Matis
 * @date 23 octobre 2023
 *
 * @param[in] df DataFrame*
 * @param[in] column_name char*
 * @return int
 *
 * @details Cette fonction permet de rechercher une colonne dans un DataFrame à partir de son nom.
 *          Elle retourne l'indice de la colonne si elle existe, -1 sinon.
 */
int findColumn(DataFrame *df, char *column_name)
{
    for (int i = 0; i < df->num_columns; i++)
    {
        if (strcmp(df->columns[i].name, column_name) == 0)
            return i;
    }
    return -1;
}

/**
 * @fn bool isIn(DataFrame *df, char *column_name, char *value)
 * @brief Fonction de recherche d'une valeur dans une colonne d'un DataFrame à partir de son nom et de sa valeur (int, double ou string uniquement)
 * @author Essengue Matis
 * @date 23 octobre 2023
 *
 * @param[in] df DataFrame*
 * @param[in] column_name char*
 * @param[in] value char*
 * @return bool
 *
 * @details Cette fonction permet de verifier si une valeur est présente dans une colonne d'un DataFrame.
 * @note Si la colonne n'existe pas, la fonction retourne false.
 */
bool isIn(DataFrame *df, char *column_name, char *value)
{
    int column_idx = findColumn(df, column_name);
    if (column_idx == -1)
    {
        fprintf(stderr, "La colonne %s n'existe pas\n", column_name);
        return false;
    }

    int *int_data;
    double *double_data;
    time_t *timestamp_data;
    char **string_data;

    for (int i = 0; i < df->num_rows; i++)
    {
        switch (df->columns[column_idx].ctype)
        {
        case INT:
            int_data = (int *)df->columns[column_idx].data;
            if (int_data[i] == atoi(value))
                return true;
            break;
        case DOUBLE:
            double_data = (double *)df->columns[column_idx].data;
            if (double_data[i] == atof(value))
                return true;
            break;
        case TIMESTAMP:
            timestamp_data = (time_t *)df->columns[column_idx].data;
            if (timestamp_data[i] == atol(value))
                return true;
            break;
        case STRING:
            string_data = (char **)df->columns[column_idx].data;
            if (strcmp(string_data[i], value) == 0)
                return true;
            break;
        }
    }
}

// TODO: fonction select (nom de colonne en param), equivalent de ['nom_colonne'] en python
// TODO: fonction where (avec un filtre)
// TODO: fonction map (avec une fonction de transformation)
// TODO: fonction apply (avec une fonction de transformation)
// TODO: fonction copy (copie d'un DataFrame)
// TODO: fonction drop (suppression d'une colonne)
