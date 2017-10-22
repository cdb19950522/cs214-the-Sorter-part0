/*
gcc sorter.c -o sorter && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./sorter -c food < i
gcc sorter.c -o sorter && ./sorter -c food < i
gcc sorter.c -o sorter && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./sorter -c duration < m.csv
gcc sorter.c -o sorter && ./sorter -c duration < m.csv
*/
#include "sorter.h"
int collen = 0, linelen = 0, tosort = -1, maxlen = 6000;
char ***table, **column, coltype;       // coltype: s for string, n for numbers
char isnum(char *s)
{
    if (strlen(s) == 0)
        return 's';
    int i;

    if ((s[0] >= '0' && s[0] <= '9') || s[0] == '-' || s[0] == '.') {
        for (i = 1; i < strlen(s); i++)
            if ((s[i] < '0' || s[i] > '9') && s[i] != '.')
                return 's';
    } else
        return 's';
    return 'n';
}

void readdata()
{
    table = (char ***)malloc(sizeof(char **) * maxlen);
    char c, *buffer = (char *)malloc(sizeof(char) * 10000);
    int i, lineindex = 0, colindex = 0, lastletterindex;

    do {
        table[lineindex] = (char **)malloc(sizeof(char *) * collen);
        colindex = 0;
        do {                    // read a line
            i = 0;
            c = getchar();
            //====================================================================//
            if (c == '"') {     // process double quotes
                do {
                    c = getchar();
                    buffer[i++] = c;
                } while (c != '"');
                buffer[i - 1] = '\0';
            }
            //====================================================================//
            while (c == ' ')    // process trailing space
                c = getchar();
            lastletterindex = -1;
            while (c != ',' && c != '\n' && c != EOF) { // read a column
                if (c != ' ')
                    lastletterindex = i;
                buffer[i++] = c;
                c = getchar();
            }
            buffer[lastletterindex + 1] = '\0';
            //====================================================================//
            if (lineindex == 0 && colindex == tosort)   // determine column type
                coltype = isnum(buffer);
            //====================================================================//
            table[lineindex][colindex] =
                (char *)malloc(sizeof(char) * strlen(buffer) + 1);
            strcpy(table[lineindex][colindex], buffer);
            colindex++;
        } while (c != '\n' && c != EOF);
        if (lineindex == maxlen - 1) {  // need to allocate more space
            maxlen += maxlen;
            table = (char ***)realloc(table, maxlen * sizeof(char **));
        }
        lineindex++;
    } while (c != EOF);
    linelen = lineindex - 1;
    free(table[linelen][0]);    // free column 0 in extra line
    free(table[linelen]);       // free extra line
    free(buffer);
}
int main(int argc, char **argv)
{
    if (argc < 3 || strcmp(argv[1], "-c") != 0) {
        printf("invalid input\n");
        return 0;
    }
    //========================================================================
    // read column titles and data
    int i, t;
    char *token, *line = (char *)malloc(sizeof(char) * 50000);
    column = (char **)malloc(sizeof(char *) * maxlen);
    fgets(line, 50000, stdin);
    token = strtok(line, ",");
    while (token != NULL) {
        column[collen] = (char *)malloc(sizeof(char) * strlen(token) + 1);
        if (tosort == -1 && strcmp(token, argv[2]) == 0)
            tosort = collen;
        if (collen == maxlen - 1) {     // need to allocate more space
            maxlen += maxlen;
            column = (char **)realloc(column, maxlen * sizeof(char *));
        }
        strcpy(column[collen++], token);
        token = strtok(NULL, ",");
    }
    free(line);
    if (tosort == -1 && strlen(argv[2]) + 2 == strlen(column[collen - 1])) {    // check last column
        tosort = collen - 1;
        for (i = 0; i < strlen(argv[2]); i++)
            if (argv[2][i] != column[collen - 1][i]) {
                tosort = -1;
                break;
            }
    }
    for (i = 0; i < collen - 1; i++) {
        printf("%s,", column[i]);
        free(column[i]);
    }
    printf("%s", column[i]);
    free(column[i]);
    free(column);
    if (tosort == -1) {
        printf("target column not found\n");
        return 0;
    }
    readdata();
    //========================================================================
    mergesort(table, 0, linelen - 1, coltype, collen, tosort);
    for (i = 0; i < linelen; i++) {
        // printf("%s\n", table[i][tosort]);
        for (t = 0; t < collen - 1; t++) {
            if (strchr(table[i][t], ','))
                printf("\"%s\",", table[i][t]);
            else
                printf("%s,", table[i][t]);
            free(table[i][t]);
        }
        if (strchr(table[i][t], ','))
            printf("\"%s\"\n", table[i][t]);
        else
            printf("%s\n", table[i][t]);
        free(table[i][t]);
        free(table[i]);
    }
    free(table);
    return 0;
}
