/**
 * @file lecture_csv.c
 * @brief Gestion et création des structures de données DataFrame à partir de fichiers CSV.
 * @author Essengue Matis
 * @date 22 octobre 2023
 *
 * Fonctions de gestion et de création des structures de données DataFrame à partir de fichiers CSV.
 *
 */

#include "lecture_csv.h"

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
        i++;
    }
    return ' ';

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
 * @fn static char *timestampToStr(time_t timestamp)
 * @brief Fonction de conversion d'un timestamp en chaîne de caractères
 * @param[in] time_t Timestamp
 * @return char*
 */
static char *timestampToStr(time_t timestamp)
{
    char *str = malloc(sizeof(char) * 20);
    strftime(str, 20, "%d/%m/%Y %H:%M:%S", localtime(&timestamp));
    return str;
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
 * @fn
 * @brief Fonction qui retourne une sous-chaîne d'une chaîne de caractères
 * @param[in] str char*
 * @param[in] start int
 * @param[in] end int
 * @return char*
 */
static char *substr(char *str, int start, int end)
{
    char *sub_str = (char *)malloc(sizeof(char) * (end - start + 1));
    int i = 0;
    for (int j = start; j < end; j++)
    {
        if (str[j] == ' ' && j == start)
            continue;
        sub_str[i] = str[j];
        i++;
    }
    sub_str[i] = '\0';
    return sub_str;
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
    {
        if (strstr(data[j], "Q01->") != NULL)
        {
            char *sub_str = substr(data[j], 9, strlen(data[j]));
            df->columns[j].name = strdup(sub_str);
        }
        else if (strstr(data[j], "Q00_") != NULL)
        {
            char *sub_str = substr(data[j], 14, strlen(data[j]));
            df->columns[j].name = strdup(sub_str);
        }
        else
        {
            df->columns[j].name = strdup(data[j]);
        }
    }
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
                printf("%s\t", timestampToStr(timestamp_data[row]));
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
 * @fn int isIn(DataFrame *df, char *column_name, char *value)
 * @brief Fonction de recherche d'une valeur dans une colonne d'un DataFrame à partir de son nom et de sa valeur (int, double ou string uniquement)
 * @param[in] df DataFrame*
 * @param[in] column_name char*
 * @param[in] value char*
 * @return int
 *
 * @details Cette fonction permet de verifier si une valeur est présente dans une colonne d'un DataFrame.
 *          Si elle est présente, la fonction retourne l'indice de la ligne, -1 sinon.
 * @note Si la colonne n'existe pas, la fonction retourne false.
 */
int isIn(DataFrame *df, char *column_name, char *value)
{
    int column_idx = findColumn(df, column_name);
    if (column_idx == -1)
    {
        fprintf(stderr, "La colonne %s n'existe pas\n", column_name);
        return -1;
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
                return i;
            break;
        case DOUBLE:
            double_data = (double *)df->columns[column_idx].data;
            if (double_data[i] == atof(value))
                return i;
            break;
        case TIMESTAMP:
            timestamp_data = (time_t *)df->columns[column_idx].data;
            time_t test_value;
            strToTimestamp(value, &test_value);
            if (timestamp_data[i] == test_value)
                return i;
            break;
        case STRING:
            string_data = (char **)df->columns[column_idx].data;
            if (strcmp(string_data[i], value) == 0)
                return i;
            break;
        }
    }

    return -1;
}

/**
 * @fn void getColumnsNames(DataFrame *df, char *columns_names[df->num_columns])
 * @brief Fonction de récupération des noms des colonnes d'un DataFrame
 * @param[in] df src
 * @param[out] columns_names dest
 */
void getColumnsNames(DataFrame *df, char *columns_names[])
{
    for (int i = 0; i < df->num_columns; i++)
        columns_names[i] = df->columns[i].name;
}

/**
 * @fn Column dfSelect(DataFrame *df, char *column_name)
 * @brief Fonction qui retourne une colonne d'un DataFrame à partir de son nom
 * @param[in] df src
 * @param[in] column_name nom de la colonne à récupérer
 * @return Column
 */
Column dfSelect(DataFrame *df, char *column_name)
{
    int idx_column = findColumn(df, column_name);

    if (idx_column == -1)
    {
        fprintf(stderr, "La colonne %s n'existe pas\n", column_name);
        exit(1);
    }

    return df->columns[idx_column];
}

/**
 * @fn Series getRow(DataFrame *df, char *column_name, char *value)
 * @brief Fonction qui retourne une ligne d'un DataFrame à partir d'une des valeurs de la ligne
 * @param[in] df src
 * @param[in] column_name nom de la colonne à récupérer
 * @param[in] value valeur de la colonne pour localiser la ligne
 * @return Series
 */
Series getRow(DataFrame *df, char *column_name, char *value)
{
    int idx_row = isIn(df, column_name, value);

    if (idx_row == -1)
    {
        fprintf(stderr, "La valeur %s n'existe pas dans la colonne %s\n", value, column_name);
        exit(1);
    }

    Series row;
    row.nb_items = df->num_columns;

    Item *items = (Item *)malloc(sizeof(Item) * row.nb_items);
    for (int i = 0; i < row.nb_items; i++)
    {
        Item item;
        Column column = dfSelect(df, df->columns[i].name);
        item.label = column.name;
        item.type = column.ctype;
        switch (item.type)
        {
        case INT:
            item.value.int_value = ((int *)column.data) + idx_row;
            break;
        case DOUBLE:
            item.value.double_value = ((double *)column.data) + idx_row;
            break;
        case TIMESTAMP:
            item.value.timestamp_value = ((time_t *)column.data) + idx_row;
            break;
        case STRING:
            item.value.string_value = ((char **)column.data) + idx_row;
            break;
        }
        items[i] = item;
    }
    row.items = items;

    return row;
}

/**
 * @fn void printSeries(Series series)
 * @brief Fonction d'affichage d'une Series
 * @param[in] series
 */
void printSeries(Series series)
{
    printf("Serie de %d items\n", series.nb_items);
    for (int i = 0; i < series.nb_items; i++)
    {
        Item item = series.items[i];
        printf("'%s' : ", item.label);
        switch (item.type)
        {
        case INT:
            printf("%d\n", *item.value.int_value);
            break;
        case DOUBLE:
            printf("%lf\n", *item.value.double_value);
            break;
        case TIMESTAMP:
            printf("%s\n", timestampToStr(*item.value.timestamp_value));
            break;
        case STRING:
            printf("%s\n", *item.value.string_value);
            break;
        }
    }
}

/**
 * @fn int selectIntFromSeries(Series series, char *label)
 * @brief Fonction de récupération d'une valeur INT d'une Series à partir de son label
 * @param[in] series
 * @param[in] label
 */
int selectIntFromSeries(Series series, char *label)
{
    for (int i = 0; i < series.nb_items; i++)
    {
        Item item = series.items[i];
        if (strcmp(item.label, label) == 0)
        {
            if (item.type != INT)
            {
                fprintf(stderr, "La colonne %s n'est pas de type INT\n", label);
                exit(1);
            }
            return *item.value.int_value;
        }
    }
    fprintf(stderr, "La colonne %s n'existe pas\n", label);
    exit(1);
}

/**
 * @fn double selectDoubleFromSeries(Series series, char *label)
 * @brief Fonction de récupération d'une valeur DOUBLE d'une Series à partir de son label
 * @param[in] series
 * @param[in] label
 */
double selectDoubleFromSeries(Series series, char *label)
{
    for (int i = 0; i < series.nb_items; i++)
    {
        Item item = series.items[i];
        if (strcmp(item.label, label) == 0)
        {
            if (item.type != DOUBLE)
            {
                fprintf(stderr, "La colonne %s n'est pas de type DOUBLE\n", label);
                exit(1);
            }
            return *item.value.double_value;
        }
    }
    fprintf(stderr, "La colonne %s n'existe pas\n", label);
    exit(1);
}

/**
 * @fn time_t selectTimestampFromSeries(Series series, char *label)
 * @brief Fonction de récupération d'une valeur TIMESTAMP d'une Series à partir de son label
 * @param[in] series
 * @param[in] label
 */
time_t selectTimestampFromSeries(Series series, char *label)
{
    for (int i = 0; i < series.nb_items; i++)
    {
        Item item = series.items[i];
        if (strcmp(item.label, label) == 0)
        {
            if (item.type != TIMESTAMP)
            {
                fprintf(stderr, "La colonne %s n'est pas de type DOUBLE\n", label);
                exit(1);
            }
            return *item.value.double_value;
        }
    }
    fprintf(stderr, "La colonne %s n'existe pas\n", label);
    exit(1);
}

/**
 * @fn char *selectStringFromSeries(Series series, char *label)
 * @brief Fonction de récupération d'une valeur STRING d'une Series à partir de son label
 * @param[in] series
 * @param[in] label
 */
char *selectStringFromSeries(Series series, char *label)
{
    for (int i = 0; i < series.nb_items; i++)
    {
        Item item = series.items[i];
        if (strcmp(item.label, label) == 0)
        {
            if (item.type != STRING)
            {
                fprintf(stderr, "La colonne %s n'est pas de type STRING\n", label);
                exit(1);
            }
            return *item.value.string_value;
        }
    }
    fprintf(stderr, "La colonne %s n'existe pas\n", label);
    exit(1);
}

/**
 * @fn void deleteColumn(DataFrame *df, char *column_name)
 * @brief Fonction de suppression d'une colonne d'un DataFrame à partir de son nom
 * @param[in, out] df
 * @param[in] column_name
 */
void deleteColumn(DataFrame *df, char *column_name)
{
    int index = findColumn(df, column_name);
    if (index == -1)
    {
        fprintf(stderr, "La colonne %s n'existe pas\n", column_name);
        exit(1);
    }
    for (int i = index; i < df->num_columns - 1; i++)
    {
        df->columns[i] = df->columns[i + 1];
    }
    df->num_columns--;
}

// int main()
// {

//     char filename[] = "jugement.csv";
//     char path[] = "../data/";
//     strcat(path, filename);
//     DataFrame *df = createDataFrameFromCsv(path);

//     printf("colonne 0: %s\n", columns_names[0]);

//     // printDf(df);
//     //     char column_name[] = "Soumis le :";
//     //     char val[] = "10/09/2023 10:54:53";

//     //     // printf("La colonne %s est d'indice : %d\n", column_name, findColumn(df, column_name));
//     //     // printf("La valeur \"%s\" est dans la colonne \"%s\" : %s\n", val, column_name, isIn(df, column_name, val) ? "true" : "false");

//     //     // Column cours = dfSelect(df, "Cours");
//     //     // char *DataType[] = {"INT", "DOUBLE", "TIMESTAMP", "STRING"};
//     //     // printf("La colonne %s est de type %s\n", cours.name, DataType[cours.ctype]);

//     //     // char *columns_names[df->num_columns];
//     //     // getColumnsNames(df, columns_names);
//     //     // for (int i = 0; i < df->num_columns; i++)
//     //     //     printf("%s, ", columns_names[i]);
//     //     // printf("\n\n");

//     //     Series reponse = getRow(df, column_name, val);
//     //     char *nom_complet = selectStringFromSeries(reponse, "Nom complet");
//     //     printf(" %s nom complet : %s\n", val, nom_complet);

//     //     // freeDataFrame(df);
//     //     printf("Affichage de la colonne Réponse: \n");
//     //     Column reponses = dfSelect(df, "Réponse");
//     //     Series row;
//     //     for (int i = 0; i < 5; i++)
//     //     {
//     //         char response[5];
//     //         sprintf(response, "%d", ((int *)reponses.data)[i]);
//     //         Series row = getRow(df, "Réponse", response);
//     //         printSeries(row);
//     //         // Series row = getRow(df, "Réponse", reponse_str);
//     //     }

//     //     printf("DataFrame free\n");

//     return 0;
// }