//
// Created by powehi on 6/26/22.
//

#include "node.h"

void deleteArgumentList (Argument * Head)
{
    Argument * Current;
    Argument * Next;

    for (Current=Head; Current; Current=Next)
    {
        Next = Current->next;
        free(Current->string);
        free(Current);
    }
}

void deleteArgumentSetList (ArgumentSet * Head)
{
    ArgumentSet * Current;
    ArgumentSet * Next;
    for (Current=Head; Current; Current=Next)
    {
        Next = Current->next;
        deleteArgumentList(Current->argumentList);
        free(Current);
    }
}

int getLength(Argument * Head)
{
    int count = 0;
    Argument * Current;

    for (Current=Head; Current; Current=Current->next) count++;

    return count;
}