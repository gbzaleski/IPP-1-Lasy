#ifndef PARSER_H
#define PARSER_H

// Sprawdzenie czy dwa stringi (ciągi charów) są identyczne
bool sameString(char *A, char *B);

// Bezpieczna alokacja stringa
void safeChalloc(char **tab, size_t mem);

// Parser - Funkcja sprawdza jaki rodzaj komendy reprezentuje wczytana linia
//  -1 = brak; 0 = error; 1 - wypisywanie; 2 - dodanie; 3 - usunięcie; 4 - sprawdzenie
// Oprócz tego funkcja dzieli wczytane polecenie na komende i argumenty
int analyseLine(char *line, int read, char **command, char **arg1, char **arg2, char **arg3,
        int *comLen, int *argLen1, int *argLen2, int *argLen3);

#endif //PARSER_H
