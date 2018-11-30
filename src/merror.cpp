#include <stdio.h>
#include <stdlib.h>
#include "merror.hpp"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
