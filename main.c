/*** Grzegorz B. Zaleski - 418494
Małe Zadanie - Implementacja w C
9 - 17 marca 2020, Warszawa ***/

// Define wymagany do poprawnego działania funkcji getline
#define _GNU_SOURCE

// Stała zwracana przez getline po wczytaniu ostatniej linii
#define END -1

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "struct.h"

// Wykonywanie operacji PRINT arg1 arg2
void printHandler(Node *root, char *arg1, char *arg2, int argLen1, int argLen2)
{
    if (argLen1 == 1)
    {
        printLines(root);
    }
    else
    {
        Node *pointer = root;
        while (pointer != NULL && sameString(arg1, pointer->line) == false)
            pointer = pointer->next;

        if (pointer != NULL && argLen2 == 1)
            printLines(pointer->lower);

        if (pointer != NULL && argLen2 > 1)
        {
            Node *middlePointer = pointer->lower;
            while(middlePointer != NULL && sameString(arg2, middlePointer->line) == false)
                middlePointer = middlePointer->next;

            if (middlePointer != NULL)
                printLines(middlePointer->lower);
        }
    }
}

// Wykonywanie operacji ADD arg1 arg2 arg3
void addHandler(Node **root, char *arg1, char *arg2, char *arg3, int argLen1, int argLen2, int argLen3)
{
    add(root, arg1, argLen1, NULL, NULL);

    if (argLen2 != 1)
    {
        Node *pointer = *root;
        while (sameString(arg1, pointer->line) == false)
            pointer = pointer->next;

        Node *middlePointer = pointer->lower;
        add(&middlePointer, arg2, argLen2, pointer, NULL);

        if (argLen3 != 1)
        {
            middlePointer = pointer->lower;
            while (sameString(arg2, middlePointer->line) == false)
                middlePointer = middlePointer->next;

            Node *downPointer = middlePointer->lower;
            add(&downPointer, arg3, argLen3, middlePointer, NULL);
        }
    }
    printf("OK\n");
}

// Wykonywanie operacji DEL arg1 arg2 arg3
void delHandler(Node **root, char *arg1, char *arg2, char *arg3, int argLen1, int argLen2, int argLen3)
{
    if (argLen1 == 1)
    {
        relinquish(*root);
        *root = NULL;
    }
    else
    {
        if (argLen2 == 1)
        {
            del(root, arg1, NULL, NULL);
        }
        else
        {
            Node *pointer = *root;
            while (pointer != NULL && sameString(arg1, pointer->line) == false)
                pointer = pointer->next;

            if (pointer != NULL)
            {
                Node *middlePointer = pointer->lower;
                if (argLen3 == 1)
                {
                    del(&middlePointer, arg2, pointer, NULL);
                }
                else
                {
                    while (middlePointer != NULL && sameString(arg2, middlePointer->line) == false)
                        middlePointer = middlePointer->next;

                    if (middlePointer != NULL)
                    {
                        Node *downPointer = middlePointer->lower;
                        del(&downPointer, arg3, middlePointer, NULL);
                    }
                }
            }
        }
    }
    printf("OK\n");
}

// Wykonywanie operacji CHECK arg1 arg2 arg3
void existHandler(Node *root, char *arg1, char *arg2, char *arg3, int argLen2, int argLen3)
{
    bool answer = false;
    Node *pointer = root;

    while (answer == false && pointer != NULL)
    {
        if (sameString(arg1, pointer->line) || sameString(arg1, "*"))
        {
            if (argLen2 == 1)
            {
                answer = true;
            }
            else
            {
                Node *middlePointer = pointer->lower;
                while (answer == false && middlePointer != NULL)
                {
                    if (sameString(arg2, middlePointer->line) || sameString(arg2, "*"))
                    {
                        if (argLen3 == 1)
                        {
                            answer = true;
                        }
                        else
                        {
                            Node *downPointer = middlePointer->lower;
                            while (answer == false && downPointer != NULL)
                            {
                                if (sameString(arg3, downPointer->line))
                                    answer = true;
                                downPointer = downPointer->next;
                            }
                        }
                    }
                    middlePointer = middlePointer->next;
                }
            }
        }
        pointer = pointer->next;
    }

    if (answer)
        printf("YES\n");
    else
        printf("NO\n");
}

// Główna część programu - wykonywanie działań zadanych przez wczytane komendy
int main()
{
    // Deklaracja zmiennych używanych przez program
    size_t mem;
    char *line = NULL, *command = NULL, *arg1 = NULL, *arg2 = NULL, *arg3 = NULL;
    Node *root = NULL;
    int comLen = 0, argLen1 = 0, argLen2 = 0, argLen3 = 0, read, dir;

    // Wczytywanie kolejnych linii poleceń aż do momentu wczytanie End of File.
    while ((read = getline(&line, &mem, stdin)) != END)
    {
        // Analiza podanego polecenia
        dir = analyseLine(line, read, &command, &arg1, &arg2, &arg3, &comLen, &argLen1, &argLen2, &argLen3);

        // Error
        if (dir == 0)
            fprintf(stderr,"ERROR\n");
        // Wypisywanie (Print)
        else if (dir == 1)
            printHandler(root, arg1, arg2, argLen1, argLen2);
        // Dodawanie elementu
        else if (dir == 2)
            addHandler(&root, arg1, arg2, arg3, argLen1, argLen2, argLen3);
        // Usunięcie elementu
        else if (dir == 3)
            delHandler(&root, arg1, arg2, arg3, argLen1, argLen2, argLen3);
        // Sprawdzanie
        else if (dir == 4)
            existHandler(root, arg1, arg2, arg3, argLen2, argLen3);

        // Przygotowanie pamięci pod kolejne polecenie
        free(command);
        free(arg1);
        free(arg2);
        free(arg3);
   }

    // Oczyszczenie pamieci pod koniec programu
    free(line);
    relinquish(root);
    return 0;
}
