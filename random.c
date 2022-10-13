//
// Created by powehi on 6/22/22.
//

#include "random.h"


int randomInt(void)
{
    return rand() - RAND_MAX/2;
}

unsigned int randomUnsignedInt(void)
{
    return rand();
}

int randomLimitedInt(int bottom, int top)
{
    if (bottom>top) return 0;
    if (bottom==top) return top;

    return rand()%(top-bottom) + bottom;
}

double randomDouble(void)
{
    int a, b, sign;
    a = rand();
    b = rand();

    while (b==0) b=rand();

    sign = rand();

    return (sign%2==1 ? -1 : 1)*((double)a)/b;
}
/*
double randomLimitedDouble(int bottom, int top)
{
    int a, b, sign=1;
    double res;

    if (bottom==top) return top;
    b = rand();
    while (b==0) b=rand();

    a = rand()%((bottom-top)*b) + bottom;
    res = ((double)a)/b;

    while (res<bottom || res>top)
    {
        a = rand()%((bottom-top)*b) + bottom;
        res = ((double)a)/b;
    }

    return (sign%2==1 ? -1 : 1)*(((double)a)/b);
} */

char randomChar(void)
{
    return randomLimitedInt(0, 256);
}

char randomLimitedChar(char bottom, char top)
{
    return randomLimitedInt(bottom, top);
}

char * randomLimitedString(char * buf, size_t limit)
{
    size_t i;

    limit = MIN(limit, sizeof(buf));
    for (i=0; i<limit; i++)
    {
        buf[i] = randomChar();
    }

    return buf;

}




















