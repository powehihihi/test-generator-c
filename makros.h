//
// Created by powehi on 6/22/22.
//

#ifndef TESTGENERATOR_MAKROS_H
#define TESTGENERATOR_MAKROS_H

#endif //TESTGENERATOR_MAKROS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MIN(X, Y) ((X)>(Y) ? (Y) : (X))

#define LENGTH 1234
#define RANDOM_STRING_LENGTH 16

enum Boolean
{
    TRUE = 1,
    FALSE = 0
};
enum Errors
{
    SUCCESS = 0,
    ERROR_LEN = -1, //  используется в read.c: analyzeAndChangeString,
                    // если в файле слишком длинная строка (длмннее LENGTH=1234)

    ERROR_OPEN = -2,
    ERROR_READ = -3,
    ERROR_MALLOC = -4,
    ERROR_GETINFO_BRACKET = -5,
    ERROR_GETINFO_TIMEOUT = -6,
    ERROR_READFILE_ARGUMENT1 = -7,  // нет аргументов в наборе аргументов и
                                    // нет закрывающей скобки } в конце файла

    ERROR_READFILE_ARGUMENT2 = -8,
    ERROR_READFILE_ARGUMENT3 = -9,
    ERROR_READTYPE = -10,       //  используется в read.c: analyzeAndChangeString,
                                // если аргумент не является ни комментарием, ни типом

    ERROR_EMPTY = -11,
    ERROR_READPARAMS = -12,     //  используется в read.c: analyzeAndChangeString,
                                // если параметры аргумента (диапазон значений и тд)
                                // указаны неправильно

    ERROR_ANALYZE = -13, //  используется в read.c: analyzeAndChangeString,
                         //  если строка не удовлетворяет синтаксису

    ERROR_TIMEOUT_SYNTAX = -14, // после !timeout требуется написать строку вида
                                // "<положительное целое число><единица измерения>"
                                // где единица измерения - 's', 'm', 'h', 'd'

    ERROR_OUT_SYNTAX = -15, //  после !out требуется ввести непустую строку без пробелов

    ERROR_NUMBER_OF_TESTS = -16, //после закрывающей скобки } требуется положительное число,
                                 // означающее число тестов для этого набора аргументов



};

enum Typenames
{
    NONE = 48,   // '0'
    INT = 49,   // '1'
    CHAR = 50,  // '2'
    DOUBLE = 51,    // '3'
    STRING = 52,    // '4'
    FILENAME = 53,  // '5'
};

typedef char boolean;
