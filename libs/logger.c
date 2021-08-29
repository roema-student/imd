#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#define MAXLINE 4096

void logger(const char *tag, const char *fmt, ...)
{
    char buf[MAXLINE + 1];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    time_t now;
    time(&now);
    printf("%s [%s]: %s\n", ctime(&now), tag, buf);
    va_end(ap);
}