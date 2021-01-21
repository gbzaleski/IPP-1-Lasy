#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

// Sprawdzenie czy dwa stringi (ciągi charów) są identyczne
bool sameString(char *A, char *B)
{
    return strcmp(A, B) == 0;
}

// Niedozwolony znak w poleceniu zadania
static inline bool wrongSign(char c)
{
    unsigned short _c = c;
    return _c < 33;
}

// Czy string zawiera same spacje
static inline bool allSpaces(const char *line, int read)
{
    for (int i=0; i<read; ++i)
        if (isspace(line[i]) == false)
            return false;
    return true;
}

// Bezpieczna alokacja stringa
void safeChalloc(char **tab, size_t mem)
{
    *tab = malloc(mem);
    if (mem > 0 && *tab == NULL)
        exit(1);
}

// Parser - Funkcja sprawdza jaki rodzaj komendy reprezentuje wczytana linia
//  -1 = brak; 0 = error; 1 - wypisywanie; 2 - dodanie; 3 - usunięcie; 4 - sprawdzenie
// Oprócz tego funkcja dzieli wczytane polecenie na komende i argumenty
int analyseLine(char *line, int read, char **command, char **arg1, char **arg2, char **arg3,
                int *comLen, int *argLen1, int *argLen2, int *argLen3)
{
    // Alokacja pamięci na komendę i argumenty
    size_t mem = read * sizeof(char);
    safeChalloc(command, mem);
    safeChalloc(arg1, mem);
    safeChalloc(arg2, mem);
    safeChalloc(arg3, mem);

    // Sprawdzenie skrajnych przypadków
    if (read == 0 || line[0] == '#' || allSpaces(line, read))
        return -1;
    else if (line[read - 1] != '\n')
        return 0;

    for (int i=0; i<read; ++i)
        if (wrongSign(line[i]) && !isspace(line[i]))
            return 0;

    // Krojenie linii z getline'a na komendę i argumenty
    // Stworzenie pomocniczych zmiennych liczących długość każdego argumentu i komendy
    int i = 0;
    int cl = 0, al1 = 0, al2 = 0, al3 = 0;

    while (i < read && isspace(line[i]))
        i++;
    while (i < read && isspace(line[i]) == false)
        (*command)[cl++] = line[i++];

    while (i < read && isspace(line[i]))
        i++;
    while (i < read && isspace(line[i]) == false)
        (*arg1)[al1++] = line[i++];

    while (i < read && isspace(line[i]))
        i++;
    while (i < read && isspace(line[i]) == false)
        (*arg2)[al2++] = line[i++];

    while (i < read && isspace(line[i]))
        i++;
    while (i < read && isspace(line[i]) == false)
        (*arg3)[al3++] = line[i++];

    while (i < read && isspace(line[i]))
        i++;
    if (i != read)
        return 0;

    // Dodanie końcowki umożliwie poprawne porównywanie stringów
    (*command)[cl++] = '\0';
    (*arg1)[al1++] = '\0';
    (*arg2)[al2++] = '\0';
    (*arg3)[al3++] = '\0';
    *comLen = cl;
    *argLen1 = al1;
    *argLen2= al2;
    *argLen3 = al3;

    // Sprawdzenie co reprezentuje uzyskana komenda wraz z jej argumentami
    if (sameString(*command, "PRINT") && al3 == 1)
    {
        return 1;
    }
    else if (sameString(*command, "ADD") && al1 > 1)
    {
        return 2;
    }
    else if (sameString(*command, "DEL"))
    {
        return 3;
    }
    else if (sameString(*command, "CHECK") && al1 > 1)
    {
        if (sameString(*arg3, "*"))
            return  0;
        else if (sameString(*arg2, "*") && al3 == 1)
            return  0;
        else if (sameString(*arg1, "*") && al3 == 1 && al2 == 1)
            return  0;
        else
            return 4;
    }
    else
    {
        return 0;
    }
}