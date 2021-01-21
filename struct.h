#ifndef STRUCT_H
#define STRUCT_H

// Deklaracja struktury do trzymania lasów, drzew i zwierząt
// Struktura posiada stringa line - nazwę elementu, oraz read - długość nazwy
// Pozatym zawiera dwa wskaźniki - na kolejny element (np. kolejne drzewo)
// oraz na element niżej (np. z drzewa na zwierzę)
typedef struct Node
{
    int read;
    char *line;
    struct Node *next;
    struct Node *lower;
} Node;

// Sprawdzenie czy string A jest leksykograficznie przed stringiem B
bool preLex(char *A, char *B);

// Czyszczenie całej pamięci od podanego elementu
void relinquish(Node *current);

// Wypisywanie wszystkich elementów na tym samym poziomie od podanego elementu
void printLines(Node *current);

// Wczytania stringa do elementu struktury
void upload(Node *current, char *line, int read);

// Dodanie kolejnego elementu na tym samym poziomie
// Funkcja trzyma elementy leksykograficznie oraz ignoruje powtórzenia
void add(Node **current, char *line, int read, Node *upper, Node *prev);

// Usunięcie elementu o podanej nazwie - o ile istnieje
void del(Node **current, char *line, Node *upper, Node *prev);

#endif //STRUCT_H
