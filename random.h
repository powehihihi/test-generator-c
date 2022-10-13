//
// Created by powehi on 6/22/22.
//

#ifndef TESTGENERATOR_RANDOM_H
#define TESTGENERATOR_RANDOM_H

#endif //TESTGENERATOR_RANDOM_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "makros.h"

int randomInt(void);
unsigned int randomUnsignedInt(void);
int randomLimitedInt(int bottom, int top);
double randomDouble(void);
//double randomLimitedDouble(int bottom, int top);
char randomChar(void);
char randomLimitedChar(char bottom, char top);
char * randomLimitedString(char * buf, size_t limit);

