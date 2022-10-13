//
// Created by powehi on 6/24/22.
//

#ifndef TESTGENERATOR_READ_H
#define TESTGENERATOR_READ_H

#endif //TESTGENERATOR_READ_H

#include "write.h"

int readFile(const char * filename, boolean * cleverTestIndicator, char ** timeout, char ** out, ArgumentSet ** Set);
int analyzeAndChangeString(char * buf);

#define COMMENT (1)
