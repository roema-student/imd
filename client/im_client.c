#include <stdio.h>
#include "logger.h"

int main() 
{
    logger(LOG_DEBUG, "Hello, client");
    return 0;
}