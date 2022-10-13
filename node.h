//
// Created by powehi on 6/26/22.
//


// Программы запускаются с набором аргументов - ArgumentSet.
// Набор аргументов состоит из аргументов - Argument, находящихся в определённом порядке.
// Таким образом ArgumentSet содержит указатель на односвязный список аргументов.
// У программы может быть несколько допустимых наборов аргументов.
// Поэтому TestGenerator формирует односвязный список из ArgumentSet.




#ifndef TESTGENERATOR_NODE_H
#define TESTGENERATOR_NODE_H

#endif //TESTGENERATOR_NODE_H


#include <stdio.h>
#include <stdlib.h>

struct argument;
typedef struct argument
{
    char * string;
    struct argument * next;
} Argument;

struct argumentSet;
typedef struct argumentSet
{
    Argument * argumentList;
    int numberOfTests;
    struct argumentSet * next;
} ArgumentSet;




void deleteArgumentList (Argument * head);
void deleteArgumentSetList (ArgumentSet * head);
int getLength(Argument * Head);
