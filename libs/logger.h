#ifndef LOGGER_H
#define LOGGER_H

#define LOG_DEBUG "DEBUG"
#define LOG_INFO "INFO"
#define LOG_WARNING "WARNING"
#define LOG_ERROR "ERROR"

void logger(const char *tag, const char *fmt, ...);

#endif /* LOG_H */