#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "struct.h"

// Sprawdzenie czy string A jest leksykograficznie przed stringiem B
bool preLex(char *A, char *B)
{
    return strcmp(A, B) < 0;
}

// Czyszczenie całej pamięci od podanego elementu
void relinquish(Node *current)
{
    if (current != NULL)
    {
        free(current->line);
        relinquish(current->next);
        relinquish(current->lower);
        free(current);
    }
}

// Wypisywanie wszystkich elementów na tym samym poziomie od podanego elementu
void printLines(Node *current)
{
    if (current != NULL)
    {
        printf("%s\n", current->line);
        printLines(current->next);
    }
};

// Bezpieczna alokacja elementu struktury
static inline void safeNalloc(Node **current)
{
    *current = malloc(sizeof(Node));
    if (*current == NULL)
        exit(1);
}

// Wczytania stringa do elementu struktury
void upload(Node *current, char *line, int read)
{
    safeChalloc(&(current->line), read * sizeof(char));
    current->read = read;
    strcpy(current->line, line);
    current->next = NULL;
    current->lower = NULL;
}

// Dodanie kolejnego elementu na tym samym poziomie
// Funkcja trzyma elementy leksykograficznie oraz ignoruje powtórzenia
void add(Node **current, char *line, int read, Node *upper, Node *prev)
{
    if (*current == NULL)
    {
        safeNalloc(current);
        upload(*current, line, read);
        if (upper != NULL)
            upper->lower = *current;
        if (prev != NULL)
            prev->next = *current;
    }
    else if (sameString(line, (*current)->line))
        return;
    else if (preLex(line, (*current)->line))
    {
        Node *newOne;
        safeNalloc(&newOne);
        upload(newOne, line, read);
        newOne->next = *current;
        if (prev != NULL)
            prev->next = newOne;
        if (upper != NULL)
            upper->lower = newOne;
        if (prev == NULL && upper == NULL)
            *current = newOne;
    }
    else
    {
        Node *nextOne = (*current)->next;
        add(&nextOne, line, read, NULL, *current);
    }
}

// Usunięcie elementu o podanej nazwie - o ile istnieje
void del(Node **current, char *line, Node *upper, Node *prev)
{
    if (*current == NULL)
        return;
    else if (sameString((*current)->line, line))
    {
        Node *nextOne = (*current)->next;
        free((*current)->line);
        relinquish((*current)->lower);
        free(*current);

        if (prev != NULL)
            prev->next = nextOne;
        if (upper != NULL)
            upper->lower = nextOne;
        if (prev == NULL && upper == NULL)
            *current = nextOne;
    }
    else if (preLex((*current)->line, line))
    {
        Node *nextOne = (*current)->next;
        del(&nextOne, line, NULL, *current);
    }
}